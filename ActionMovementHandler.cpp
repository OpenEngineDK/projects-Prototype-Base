// Basic movement handler.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "ActionMovementHandler.h"

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

		ActionMovementHandler::ActionMovementHandler(Tank* tank, IMouse* m, FixedTimeStepPhysics* physics, int midX, int midY) : tank(tank), mouse(m),
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
		}

		ActionMovementHandler::~ActionMovementHandler() {}

		void ActionMovementHandler::Initialize() {
			mouse->ShowCursor();
		}

		void ActionMovementHandler::Deinitialize() {}

		bool ActionMovementHandler::IsTypeOf(const std::type_info& inf) { 
			return typeid(ActionMovementHandler) == inf; 
		}

		void ActionMovementHandler::MouseMoved(MouseMovedEventArg arg) {
			if ( mouse->IsPressed(BUTTON_RIGHT) ) {
				Vector<3,float> yAxis(0,1,0);
				float rSpeed = 0.005;
				float mSpeed = 0.5;
				mouse->HideCursor();
				if ( mouse3Down ) {

				} else {
					middleX = arg.x;
					middleY = arg.y;
					mouse3Down = true;
				}
				tank->GetTurretTransformationNode()->Rotate(0, ((float)middleX-(float)arg.x)*rSpeed, 0);
				Quaternion<float> rotation = tank->GetTurretGunTransformationNode()->GetRotation() * Quaternion<float>(0, 0, ((float)middleY-(float)arg.y)*rSpeed);
				if ( rotation.GetImaginary()[2] > 0.25  ) {
					rotation = Quaternion<float>(0.0, 0.0, 0.5);
				} else if ( rotation.GetImaginary()[2] < -0.1 ) {
					rotation = Quaternion<float>(0.0, 0.0, -0.2);
				}
				tank->GetTurretGunTransformationNode()->SetRotation(rotation);
				mouse->SetCursor(middleX,middleY);
			} else {
				if ( mouse3Down ) {
					mouse3Down = false;
					mouse->ShowCursor();
				}
			}
		}

		void ActionMovementHandler::Process(const float dt, const float percent) {
			RigidBox* box = tank->GetRigidBox();
			if( box == NULL ) return;
			static float speed = 300.0f;
			static float turn = 550.0f;
			Matrix<3,3,float> m; //orientation
			if( forward || back || left || right ) {
				float delta = dt / 1000 * 8;
				Vector<3,float> dir = Vector<3,float>();;
				m = box->GetRotationMatrix();
				if( forward ){
					dir += Vector<3,float>(1.0, 0.0, 0.0);				
				}
				if( back ){
					dir += Vector<3,float>(-1.0, 0.0, 0.0);
				}
				if( left ){
					dir += Vector<3,float>(0.0, 0.0, -1.0);	
				}
				if( right ) {
					dir += Vector<3,float>(0.0, 0.0, 1.0);
				}
				dir = tank->GetTurretRotation().RotateVector(dir);
				box->AddForce(dir * speed, 1);
				box->AddForce(dir * speed, 2);
				box->AddForce(dir * speed, 3);
				box->AddForce(dir * speed, 4);
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
		void ActionMovementHandler::HandleDownEvent(KeyboardEventArg arg) { 
			HandleKeyEvent(arg, true); 
		}
		void ActionMovementHandler::HandleUpEvent(KeyboardEventArg arg) { 
			HandleKeyEvent(arg, false); 
		}
		void ActionMovementHandler::HandleKeyEvent(KeyboardEventArg arg, bool state) {
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

		void ActionMovementHandler::HandleMouseDown(MouseButtonEventArg event) {
			mLeftClick = (event.button == BUTTON_LEFT);
			mRightClick = (event.button == BUTTON_RIGHT);
			mWheelUp = (event.button == BUTTON_WHEEL_UP);
			mWheelDown = (event.button == BUTTON_WHEEL_DOWN);
		}

		void ActionMovementHandler::BindToEventSystem() {
			Listener<ActionMovementHandler,KeyboardEventArg>* downl =
				new Listener<ActionMovementHandler,KeyboardEventArg>(*this,&ActionMovementHandler::HandleDownEvent);
			IKeyboard::keyDownEvent.Add(downl);

			Listener<ActionMovementHandler,KeyboardEventArg>* upl =
				new Listener<ActionMovementHandler,KeyboardEventArg>(*this,&ActionMovementHandler::HandleUpEvent);
			IKeyboard::keyUpEvent.Add(upl);

			Listener<ActionMovementHandler, MouseButtonEventArg>* mouseDownListener = 
				new Listener<ActionMovementHandler, MouseButtonEventArg>(*this, &ActionMovementHandler::HandleMouseDown);
			IMouse::mouseDownEvent.Add(mouseDownListener);

			Listener<ActionMovementHandler, MouseMovedEventArg>* mouseMovedListener = 
				new Listener<ActionMovementHandler, MouseMovedEventArg>(*this, &ActionMovementHandler::MouseMoved);
			IMouse::mouseMovedEvent.Add(mouseMovedListener);
		}
	} // NS Utils
} // NS OpenEngine
