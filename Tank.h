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
#include "ShotManager.h"
#include "TestShotA.h"
#include "TestShotB.h"

namespace OpenEngine {
	namespace Utils {
		using namespace OpenEngine::Scene;
		using namespace OpenEngine::Physics;
		using namespace OpenEngine::Core;
		using namespace OpenEngine::Prototype;

		class Tank : public IModule {
		private:
			static GeometryNode* bodyModel;
			static GeometryNode* turretModel;
			static GeometryNode* gunModel;

			GeometryNode* tankBody;
			GeometryNode* tankTurret;
			GeometryNode* tankGun;
                        TransformationNode* cameraRotateTrans;
                        TransformationNode* cameraPivotTrans;
			TransformationNode* tankBodyTrans;
			TransformationNode* tankTurretTrans;
			TransformationNode* tankTurretGunTrans;
			RigidBox* box;
			ShotManager* shotMgr;

                        float rotation;
                        float pivot;

			Vector<3,float> position;
			Quaternion<float> direction;

		public:
			Tank(RigidBox* box);
			virtual ~Tank();

			void Initialize();
			void Deinitialize();
			virtual bool IsTypeOf(const std::type_info& inf);
			virtual void Process(const float dt, const float percent);

			static void SetModel(GeometryNode* body, GeometryNode* turret, GeometryNode* gun);

                        TransformationNode* GetCameraTransformationNode();
                        TransformationNode* GetCameraRotateTransformationNode();
			TransformationNode* GetTankTransformationNode();
			TransformationNode* GetTurretTransformationNode();
			TransformationNode* GetTurretGunTransformationNode();

			Quaternion<float> GetTurretRotation();

			RigidBox* GetRigidBox();
			void ShootPrimary();
			void ShootSecondary();
			void SetShotManager(ShotManager* shotMgr);
		};
	}
}

#endif // _Tank_H_
