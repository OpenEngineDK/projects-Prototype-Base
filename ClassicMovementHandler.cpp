// Basic movement handler.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "ClassicMovementHandler.h"


#include <Devices/IMouse.h>
#include <Math/Quaternion.h>
//#include <Physics/RigidBox.h>
#include "TankController.h"
#include <Logging/Logger.h>
using namespace OpenEngine::Logging;

namespace OpenEngine {
	namespace Prototype {

		using OpenEngine::Scene::TransformationNode;
		using namespace OpenEngine::Math;
		//using namespace OpenEngine::Physics;

		ClassicMovementHandler::ClassicMovementHandler(IMouse* m, /*FixedTimeStepPhysics* physics,*/ int midX, int midY) : mouse(m),
			forward(false), back(false),
			right(false), left(false),
			up(false), down(false) {
				offset = 100;
				middleX = midX;
				middleY = midY;
				lx = midX;
				ly = midY;
				mLeftClick = mRightClick = false;
				mWheelUp = mWheelDown = false;
				reset = false;
				//this->physics = physics;
                                turretRotation = turretPivot = 0.0;
                                cameraRotation = cameraPivot = 0.0;
                                rotationFactor = 100.0;

				tankCounter = 0;

				hackWeaponChanger = 0;
				hackWeaponChanger2 = 3;
		}

		ClassicMovementHandler::~ClassicMovementHandler() {}

		void ClassicMovementHandler::Initialize() {
			mouse->ShowCursor();
		}

		void ClassicMovementHandler::Deinitialize() {}

		bool ClassicMovementHandler::IsTypeOf(const std::type_info& inf) { 
			return typeid(ClassicMovementHandler) == inf; 
		}



		void ClassicMovementHandler::Handle(MouseMovedEventArg arg) {
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

		void ClassicMovementHandler::Handle(ProcessEventArg arg) 
                {
                  
                  //@todo: unused variable: float dt = arg.approx / 1000.0;
                        // Turret and camera handling
                        float rSpeed = 0.001;
                        //@todo: unused variable: float mSpeed = 0.5;
                        
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
			//RigidBody* box = tank->GetRigidBox();
			//if( box == NULL ) return;
			//@todo: unused variable: static float speed = 1750.0f;
			//@todo: unused variable: static float turn = 550.0f;
			Matrix<3,3,float> m; //orientation
			//if( forward || back || left || right ) m = box->GetRotationMatrix();
			//@todo: unused variable: float delta = dt / 1000 * 8;
			/*
			// Forward 
			if( forward ){
				Vector<3,float> dir = m.GetRow(0) * delta;
				box->ApplyForce(dir * speed, 1);
				box->ApplyForce(dir * speed, 2);
				box->ApplyForce(dir * speed, 3);
				box->ApplyForce(dir * speed, 4);
			}
			if( back ){
				Vector<3,float> dir = -m.GetRow(0) * delta;
				box->ApplyForce(dir * speed, 5);
				box->ApplyForce(dir * speed, 6);
				box->ApplyForce(dir * speed, 7);
				box->ApplyForce(dir * speed, 8);
			}
			if( left ){
				Vector<3,float> dir = -m.GetRow(2) * delta;
				box->ApplyForce(dir * turn, 2);
				box->ApplyForce(dir * turn, 4);
			}
			if( right ) {
				Vector<3,float> dir = m.GetRow(2) * delta;
				box->ApplyForce(dir * turn, 1);
				box->ApplyForce(dir * turn, 3);
			}
			*/
			if( up ){
				tankCounter++;
				tankCtrl->SetPlayerTank(tankCounter);
				up = false;
			}
			if( down ) {
				tankCounter--;
				tankCtrl->SetPlayerTank(tankCounter);
				down = false;
			}
			if( mLeftClick ) {
				tank->ShootGun(hackWeaponChanger);
			}
			if( mRightClick ) {
				tank->ShootGun(hackWeaponChanger2);
			}
			if( mWheelUp ) {
				hackWeaponChanger++;
				if ( hackWeaponChanger == tank->GetGunManager()->GetGunMap().size() - 2 ) {
					hackWeaponChanger = 0;
				}
				mWheelUp = false;
			}
			if( mWheelDown ) {
				hackWeaponChanger--;
				if ( hackWeaponChanger > 1000 ) {
					hackWeaponChanger = tank->GetGunManager()->GetGunMap().size() - 3;
				}
				mWheelDown = false;
			}
			if ( reset ) {
				/*
				physics->Initialize();
				if( physics != NULL ){
					if( box != NULL ) {
						box->ResetForces();
						box->SetCenter( Vector<3,float>(2, 1, 2) );
					}
				}
				*/
				reset = false;
			}
		}

		// set state of keys on up/down events
		void ClassicMovementHandler::Handle(KeyboardEventArg arg) { 
            if (arg.type == KeyboardEventArg::PRESS)
                HandleKeyEvent(arg, true); 
            else if (arg.type == KeyboardEventArg::RELEASE)
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
			case KEY_1: hackWeaponChanger	= 0; break;
			case KEY_2: hackWeaponChanger	= 1; break;
			case KEY_3: hackWeaponChanger	= 2; break;
			case KEY_4: hackWeaponChanger2	= 3; break;
			case KEY_5: hackWeaponChanger2	= 4; break;
				// object changing
			default: 
				break;
				// ignore all other keys
			}
		}

		void ClassicMovementHandler::Handle(MouseButtonEventArg event) {
            if (event.type == MouseButtonEventArg::PRESS) {
                mLeftClick = (event.button == BUTTON_LEFT);
                mRightClick = (event.button == BUTTON_RIGHT);
                mWheelUp = (event.button == BUTTON_WHEEL_UP);
                mWheelDown = (event.button == BUTTON_WHEEL_DOWN);
            } else if (event.type == MouseButtonEventArg::RELEASE) {
                if (event.button == BUTTON_LEFT) 
                    mLeftClick = false;
                if (event.button == BUTTON_RIGHT)
                    mRightClick = false;
            }
        }
		void ClassicMovementHandler::BindToEventSystem(IKeyboard & key, IMouse & mouse) {

            key.KeyEvent().Attach(*this);
            mouse.MouseMovedEvent().Attach(*this);
            mouse.MouseButtonEvent().Attach(*this);
		}

		void ClassicMovementHandler::SetTank(ITank* tank) {
			this->tank = tank;
		}

		void ClassicMovementHandler::SetTankController(TankController* tankCtrl) {
			this->tankCtrl = tankCtrl;
		}
	} // NS Utils
} // NS OpenEngine
