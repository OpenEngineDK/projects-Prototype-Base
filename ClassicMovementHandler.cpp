// Basic movement handler.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "ClassicMovementHandler.h"

#include <Core/IGameEngine.h>
#include <Devices/IMouse.h>
#include <Math/Quaternion.h>
#include <Physics/RigidBox.h>

namespace OpenEngine {
	namespace Utils {

		using OpenEngine::Core::IGameEngine;
		using OpenEngine::Scene::TransformationNode;
		using namespace OpenEngine::Math;
		using namespace OpenEngine::Physics;

		ClassicMovementHandler::ClassicMovementHandler(Tank* tank, IMouse* m, FixedTimeStepPhysics* physics, int midX, int midY) : tank(tank), mouse(m),
			forward(false), back(false),
			right(false), left(false),
			up(false), down(false) {
				offset = 100;
				middleX = midX;
				middleY = midY;
				lx = midX;
				ly = midY;
				mLeftClick = mRightClick = false;
				reset = false;
				this->physics = physics;
                                turretRotation = turretPivot = 0.0;
                                cameraRotation = cameraPivot = 0.0;
                                rotationFactor = 100.0;
		}

		ClassicMovementHandler::~ClassicMovementHandler() {}

		void ClassicMovementHandler::Initialize() {
			mouse->ShowCursor();
		}

		void ClassicMovementHandler::Deinitialize() {}

		bool ClassicMovementHandler::IsTypeOf(const std::type_info& inf) { 
			return typeid(ClassicMovementHandler) == inf; 
		}

		void ClassicMovementHandler::MouseMoved(MouseMovedEventArg arg) {
			//if ( mouse->IsPressed(BUTTON_RIGHT) ) {
				mouse->HideCursor();
				if ( mouse3Down ) {

				} else {
					//middleX = arg.x;
					//middleY = arg.y;
					mouse3Down = true;
				}
                                // Turret rotation
                                turretRotation += ((float)middleX-(float)arg.x);
                                cameraRotation += ((float)middleX-(float)arg.x);
                                if ( turretRotation > PI*1000.0 ) {
                                    turretRotation -= PI*2000.0;
                                }
                                if ( turretRotation < -PI*1000.0 ) {
                                    turretRotation += PI*2000.0;
                                }
                                // Gun pivot
                                turretPivot += ((float)middleY-(float)arg.y);
                                cameraPivot += ((float)middleY-(float)arg.y);

                                //reset mouse position
				mouse->SetCursor(middleX,middleY);
			/*} else {
				if ( mouse3Down ) {
					mouse3Down = false;
					mouse->ShowCursor();
				}
			}*/
		}

		void ClassicMovementHandler::Process(const float dt, const float percent) {
                        // Turret and camera handling
                        float rSpeed = 0.001;
                        float mSpeed = 0.5;
                        
                        float rot = 0.0;
                        if ( fabs(turretRotation) > rotationFactor ) {
                            rot = rotationFactor*(turretRotation/fabs(turretRotation));
                        } else {
                            rot = turretRotation;
                        }
                        turretRotation -= rot;
                        //tank->GetTurretTransformationNode()->Rotate(0, ((float)middleX-(float)arg.x)*rSpeed, 0);
                        tank->GetTurretTransformationNode()->Rotate(0.0, rot*rSpeed, 0.0);
                        tank->GetCameraRotateTransformationNode()->Rotate(0.0, cameraRotation*rSpeed, 0.0);
                        cameraRotation = 0.0;

                        rot = 0.0;
                        if ( fabs(turretPivot) > rotationFactor ) {
                            rot = rotationFactor*(turretPivot/fabs(turretPivot));
                        } else {
                            rot = turretPivot;
                        }
                        turretPivot -= rot;
                        Quaternion<float> rotation = tank->GetTurretGunTransformationNode()->GetRotation() * Quaternion<float>(0, 0, rot*rSpeed);
                        if ( rotation.GetImaginary()[2] > 0.25  ) {
                            rotation = Quaternion<float>(0.0, 0.0, 0.5);
                        } else if ( rotation.GetImaginary()[2] < -0.03 ) {
                            rotation = Quaternion<float>(0.0, 0.0, -0.06);
                        }
                        tank->GetTurretGunTransformationNode()->SetRotation(rotation);

                        /*Quaternion<float> camRotation = tank->GetCameraTransformationNode()->GetRotation() * Quaternion<float>(0, 0, cameraPivot*rSpeed);
                        if ( camRotation.GetImaginary()[2] > 0.25  ) {
                            camRotation = Quaternion<float>(0.0, 0.0, 0.5);
                        } else if ( camRotation.GetImaginary()[2] < -0.03 ) {
                            camRotation = Quaternion<float>(0.0, 0.0, -0.06);
                        }
                        tank->GetCameraTransformationNode()->SetRotation(camRotation);
                        cameraPivot = 0.0;*/



                        // Keyboard handling
			RigidBox* box = tank->GetRigidBox();
			if( box == NULL ) return;
			static float speed = 1750.0f;
			static float turn = 550.0f;
			Matrix<3,3,float> m; //orientation
			if( forward || back || left || right ) m = box->GetRotationMatrix();

			float delta = dt / 1000 * 8;
			// Forward 
			if( forward ){
				Vector<3,float> dir = m.GetRow(0) * delta;
				box->AddForce(dir * speed, 1);
				box->AddForce(dir * speed, 2);
				box->AddForce(dir * speed, 3);
				box->AddForce(dir * speed, 4);
			}
			if( back ){
				Vector<3,float> dir = -m.GetRow(0) * delta;
				box->AddForce(dir * speed, 5);
				box->AddForce(dir * speed, 6);
				box->AddForce(dir * speed, 7);
				box->AddForce(dir * speed, 8);
			}
			if( left ){
				Vector<3,float> dir = -m.GetRow(2) * delta;
				box->AddForce(dir * turn, 2);
				box->AddForce(dir * turn, 4);
			}
			if( right ) {
				Vector<3,float> dir = m.GetRow(2) * delta;
				box->AddForce(dir * turn, 1);
				box->AddForce(dir * turn, 3);
			}
			if( mLeftClick ) {
				tank->ShootCannon();
				mLeftClick = false;
			}
			if ( reset ) {
				physics->Initialize();
				if( physics != NULL ){
					if( box != NULL ) {
						box->ResetForces();
						box->SetCenter( Vector<3,float>(2, 1, 2) );
					}
				}
				reset = false;
			}
		}

		// set state of keys on up/down events
		void ClassicMovementHandler::HandleDownEvent(KeyboardEventArg arg) { 
			HandleKeyEvent(arg, true); 
		}
		void ClassicMovementHandler::HandleUpEvent(KeyboardEventArg arg) { 
			HandleKeyEvent(arg, false); 
		}
		void ClassicMovementHandler::HandleKeyEvent(KeyboardEventArg arg, bool state) {
			switch (arg.sym) {
				// movement keys
			case KEY_w: forward = state; break;
			case KEY_s: back    = state; break;
			case KEY_a: left    = state; break;
			case KEY_d: right   = state; break;
			case KEY_e: up      = state; break;
			case KEY_q: down    = state; break;
			case KEY_r: reset	= state; break;
				// object changing
			default: 
				break;
				// ignore all other keys
			}
		}

		void ClassicMovementHandler::HandleMouseDown(MouseButtonEventArg event) {
			mLeftClick = (event.button == BUTTON_LEFT);
			mRightClick = (event.button == BUTTON_RIGHT);
			mWheelUp = (event.button == BUTTON_WHEEL_UP);
			mWheelDown = (event.button == BUTTON_WHEEL_DOWN);
		}

		void ClassicMovementHandler::BindToEventSystem() {
			Listener<ClassicMovementHandler,KeyboardEventArg>* downl =
				new Listener<ClassicMovementHandler,KeyboardEventArg>(*this,&ClassicMovementHandler::HandleDownEvent);
			IKeyboard::keyDownEvent.Add(downl);

			Listener<ClassicMovementHandler,KeyboardEventArg>* upl =
				new Listener<ClassicMovementHandler,KeyboardEventArg>(*this,&ClassicMovementHandler::HandleUpEvent);
			IKeyboard::keyUpEvent.Add(upl);

			Listener<ClassicMovementHandler, MouseButtonEventArg>* mouseDownListener = 
				new Listener<ClassicMovementHandler, MouseButtonEventArg>(*this, &ClassicMovementHandler::HandleMouseDown);
			IMouse::mouseDownEvent.Add(mouseDownListener);

			Listener<ClassicMovementHandler, MouseMovedEventArg>* mouseMovedListener = 
				new Listener<ClassicMovementHandler, MouseMovedEventArg>(*this, &ClassicMovementHandler::MouseMoved);
			IMouse::mouseMovedEvent.Add(mouseMovedListener);
		}
	} // NS Utils
} // NS OpenEngine
