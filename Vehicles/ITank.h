// Tank.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _ITank_H_
#define _ITank_H_

#include <Scene/ISceneNode.h>
#include <Scene/SceneNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>
#include <Physics/RigidBox.h>
#include "../ShotManager.h"
#include "../GunManager.h"
#include "../Weapons/LaserGun.h"
#include "../Weapons/RapidLaserGun.h"
#include "../Weapons/LightningGun.h"
#include "../Weapons/LandmineLayer.h"
#include "../Weapons/ShieldGenerator.h"

namespace OpenEngine {
	namespace Prototype {
            namespace Vehicles {
		using namespace OpenEngine::Scene;
		using namespace OpenEngine::Physics;
                using namespace OpenEngine::Prototype;
                using namespace OpenEngine::Prototype::Weapons;

		class ITank {
		protected:
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
			GunManager* gunMgr;

            float rotation;
            float pivot;

			Vector<3,float> position;
			Quaternion<float> direction;

		public:
			ITank(RigidBox* box) {};
			virtual ~ITank() {};

			virtual void Process(const float timeSinceLast) {};

			virtual TransformationNode* GetCameraTransformationNode() = 0;
			virtual TransformationNode* GetCameraRotateTransformationNode() = 0;
			virtual TransformationNode* GetTankTransformationNode() = 0;
			virtual TransformationNode* GetTurretTransformationNode() = 0;
			virtual TransformationNode* GetTurretGunTransformationNode() = 0;

			virtual RigidBox* GetRigidBox()  = 0;

			virtual void ShootGun(int i)  = 0;

			virtual void SetShotManager(ShotManager* shotMgr) {
				this->shotMgr = shotMgr;
			}
			virtual ShotManager* GetShotManager() {
				return shotMgr;
			}
			virtual GunManager* GetGunManager() {
				return gunMgr;
			}
		};
            }
	}
}

#endif // _Tank_H_
