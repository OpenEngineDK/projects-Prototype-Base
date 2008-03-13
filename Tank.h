// Tank.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _Tank_H_
#define _Tank_H_

#include <Core/IModule.h>
#include <Scene/ISceneNode.h>
#include <Scene/SceneNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>
#include <Physics/RigidBox.h>
#include "TestShot.h"

namespace OpenEngine {
	namespace Utils {
		using namespace OpenEngine::Scene;
		using namespace OpenEngine::Physics;
		using namespace OpenEngine::Core;

		class Tank : public IModule {
		private:
			GeometryNode* tankBody;
			GeometryNode* tankTurret;
			GeometryNode* tankGun;
                        TransformationNode* cameraRotateTrans;
                        TransformationNode* cameraPivotTrans;
			TransformationNode* tankBodyTrans;
			TransformationNode* tankTurretTrans;
			TransformationNode* tankTurretGunTrans;
			RigidBox* box;
			TestShot* tShot;
                        
                        float rotation;
                        float pivot;

			Vector<3,float> position;
			Quaternion<float> direction;

		public:
			Tank(GeometryNode* tankBody, GeometryNode* tankTurret, GeometryNode* tankGun, RigidBox* box);
			virtual ~Tank();

			void Initialize();
			void Deinitialize();
			virtual bool IsTypeOf(const std::type_info& inf);
			virtual void Process(const float dt, const float percent);
			
                        TransformationNode* GetCameraTransformationNode();
                        TransformationNode* GetCameraRotateTransformationNode();
			TransformationNode* GetTankTransformationNode();
			TransformationNode* GetTurretTransformationNode();
			TransformationNode* GetTurretGunTransformationNode();

			Quaternion<float> GetTurretRotation();

			RigidBox* GetRigidBox();
			void ShootCannon();
			TestShot* GetTestShot();
		};
	}
}

#endif // _Tank_H_
