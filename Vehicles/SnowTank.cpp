// Tank.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "SnowTank.h"

namespace OpenEngine {
	namespace Prototype {
            namespace Vehicles {
		using namespace OpenEngine::Math;

		// initialize static members
		GeometryNode* SnowTank::bodyModel = NULL;
		GeometryNode* SnowTank::turretModel = NULL;
		GeometryNode* SnowTank::gunModel = NULL;

		SnowTank::SnowTank(DynamicBody* dynamicBody) : ITank(dynamicBody) {
			GeometryNode* tankBody = dynamic_cast<GeometryNode*>(bodyModel->Clone());
			GeometryNode* tankTurret = dynamic_cast<GeometryNode*>(turretModel->Clone());
			GeometryNode* tankGun = dynamic_cast<GeometryNode*>(gunModel->Clone());
			this->dynamicBody = dynamicBody;

			tankBodyTrans = new TransformationNode();
			tankBodyTrans->AddNode(tankBody);

			cameraRotateTrans = new TransformationNode();
			cameraPivotTrans = new TransformationNode();
			cameraRotateTrans->AddNode(cameraPivotTrans);

			tankTurretTrans = new TransformationNode();
			tankTurretGunTrans = new TransformationNode();//
			tankTurretGunTrans->Move(0.0, 2.0, 0.0);
			tankTurretTrans->AddNode(tankTurretGunTrans);//
			tankTurretGunTrans->AddNode(tankGun);
			tankTurretTrans->AddNode(tankTurret);
			tankBodyTrans->AddNode(tankTurretTrans);//
			tankBodyTrans->AddNode(cameraRotateTrans);
			tankTurretTrans->Move(0.0, 7.0, 0.0);	

			position = Vector<3,float>();
			direction = Quaternion<float>();

			gunMgr = new GunManager(this);
			gunMgr->AddGun(new LaserGun(), GunManager::PRIMARY_WEAPON);
			gunMgr->AddGun(new RapidLaserGun(), GunManager::PRIMARY_WEAPON);
			gunMgr->AddGun(new LightningGun(), GunManager::PRIMARY_WEAPON);
			gunMgr->AddGun(new LandmineLayer(), GunManager::SECONDARY_WEAPON);	
			gunMgr->AddGun(new ShieldGenerator(), GunManager::SECONDARY_WEAPON);
		}

		SnowTank::~SnowTank() {
		}

		void SnowTank::Process(const float timeSinceLast) {
		}

		void SnowTank::SetModel(GeometryNode* body, GeometryNode* turret, GeometryNode* gun) {
			bodyModel = body;
			turretModel = turret;
			gunModel = gun;
		}

		TransformationNode* SnowTank::GetCameraTransformationNode() {
			return cameraPivotTrans;
		}

		TransformationNode* SnowTank::GetCameraRotateTransformationNode() {
			return cameraRotateTrans;
		}

		TransformationNode* SnowTank::GetTankTransformationNode() {
			return tankBodyTrans;
		}

		TransformationNode* SnowTank::GetTurretTransformationNode() {
			return tankTurretTrans;
		}

		TransformationNode* SnowTank::GetTurretGunTransformationNode() {
			return tankTurretGunTrans;
		}

		DynamicBody* SnowTank::GetDynamicBody() {
			return dynamicBody;
		}

		void SnowTank::ShootGun(int i) {
			gunMgr->ShootGun(i);
		}
            }
	}
}
