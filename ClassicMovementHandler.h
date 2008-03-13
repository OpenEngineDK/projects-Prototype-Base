// Basic movement handler.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _CLASSIC_MOVEMENT_HANDLER_H_
#define _CLASSIC_MOVEMENT_HANDLER_H_

#include <Core/IModule.h>
#include <Display/Camera.h>
#include <Devices/IKeyboard.h>
#include <Devices/IMouse.h>
#include <Scene/TransformationNode.h>
#include <Math/Vector.h>
#include <Physics/FixedTimeStepPhysics.h>
#include <vector>

#include "Tank.h"

namespace OpenEngine {
	namespace Utils {

		using OpenEngine::Math::Vector;
		using namespace OpenEngine::Core;
		using namespace OpenEngine::Display;
		using namespace OpenEngine::Scene;
		using namespace OpenEngine::Devices;
		using namespace OpenEngine::Utils;
		using namespace OpenEngine::Physics;

		/**
		* Camera movement handler
		*/
		class ClassicMovementHandler : public IModule {

		private:
			FixedTimeStepPhysics* physics;
			Tank* tank;
			IMouse* mouse;
			bool forward, back, right, left, up, down, mouse3Down;  // active move direction
			bool mWheelUp, mWheelDown;
			bool mLeftClick, mRightClick;
			bool reset;
			int lx, ly; // last mouse position

			// define the box where the mouse should be inside
			int offset;
			int middleX;
			int middleY;
                        
                        float turretRotation;
                        float turretPivot;
                        float cameraRotation;
                        float cameraPivot;
                        float rotationFactor;

		public:

			std::vector<TransformationNode*> nodes; // node list

            ClassicMovementHandler(Tank* tank, IMouse* m, FixedTimeStepPhysics* physics, int midX = 300, int midY = 300);
			~ClassicMovementHandler();
			void Initialize();
			void Deinitialize();
			virtual bool IsTypeOf(const std::type_info& inf);
			virtual void Process(const float dt, const float percent);
			virtual void HandleDownEvent(KeyboardEventArg arg);
			virtual void HandleUpEvent(KeyboardEventArg arg);
			virtual void HandleKeyEvent(KeyboardEventArg arg, bool state);
			virtual void HandleMouseDown(MouseButtonEventArg event);
			void BindToEventSystem();
			void MouseMoved(MouseMovedEventArg event);
		};
	} // NS Utils
} // NS OpenEngine

#endif // _MOVE_HANDLER_H_
