// Tank.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "Tank.h"

namespace OpenEngine {
	namespace Utils {
		using namespace OpenEngine::Math;

		// initialize static members
		GeometryNode* Tank::bodyModel = NULL;
		GeometryNode* Tank::turretModel = NULL;
		GeometryNode* Tank::gunModel = NULL;

		Tank::Tank(RigidBox* box) {
			GeometryNode* tankBody = dynamic_cast<GeometryNode*>(bodyModel->Clone());
			GeometryNode* tankTurret = dynamic_cast<GeometryNode*>(turretModel->Clone());
			GeometryNode* tankGun = dynamic_cast<GeometryNode*>(gunModel->Clone());
			this->box = box;

			tankBodyTrans = new TransformationNode();
			tankBodyTrans->AddNode(tankBody);

                        cameraRotateTrans = new TransformationNode();
                        cameraPivotTrans = new TransformationNode();
                        cameraRotateTrans->AddNode(cameraPivotTrans);

			tankTurretTrans = new TransformationNode();
			tankTurretGunTrans = new TransformationNode();//
			tankTurretGunTrans->Move(0.0, 3.0, 0.0);
                        tankTurretTrans->AddNode(tankTurretGunTrans);//
			tankTurretGunTrans->AddNode(tankGun);
			tankTurretTrans->AddNode(tankTurret);
			tankBodyTrans->AddNode(tankTurretTrans);//
                        tankBodyTrans->AddNode(cameraRotateTrans);
			tankTurretTrans->Move(0.0, 9.0, 0.0);	

			position = Vector<3,float>();
			direction = Quaternion<float>();
		}

		Tank::~Tank() {
		}

		void Tank::Initialize() {
		}
		void Tank::Deinitialize() {

		}
		bool Tank::IsTypeOf(const std::type_info& inf) {
			return false;
		}
		void Tank::Process(const float dt, const float percent) {
		}

		void Tank::SetModel(GeometryNode* body, GeometryNode* turret, GeometryNode* gun) {
			bodyModel = body;
			turretModel = turret;
        		gunModel = gun;
		}
	
                TransformationNode* Tank::GetCameraTransformationNode() {
                    return cameraPivotTrans;
                }

                TransformationNode* Tank::GetCameraRotateTransformationNode() {
                    return cameraRotateTrans;
                }

		TransformationNode* Tank::GetTankTransformationNode() {
			return tankBodyTrans;
		}

		TransformationNode* Tank::GetTurretTransformationNode() {
			return tankTurretTrans;
		}

		TransformationNode* Tank::GetTurretGunTransformationNode() {
			return tankTurretGunTrans;
		}

		RigidBox* Tank::GetRigidBox() {
			return box;
		}

		void Tank::ShootPrimary() {

			tankTurretGunTrans->GetAccumulatedTranformations(&position,&direction);

			Vector<3,float> shotDir = Vector<3,float>(20.0, 0.0, 0.0);
			Vector<3,float> posOfCannonBarrel = Vector<3,float>(0.0, 0.0, 0.0);
			
			shotDir = direction.RotateVector(shotDir) + position;
			posOfCannonBarrel = direction.RotateVector(posOfCannonBarrel) + position;

			TestShotA* shot = new TestShotA(posOfCannonBarrel,shotDir);
			shotMgr->AddShot(shot);

			if( box == NULL ) return;
			float force = 250.0;
			Vector<3,float> forceDirection = (position - shotDir).GetNormalize();
			box->AddForce(forceDirection * force, 1);
			box->AddForce(forceDirection * force, 2);
			box->AddForce(forceDirection * force, 5);
			box->AddForce(forceDirection * force, 6);

			box->AddForce(-forceDirection * force, 3);
			box->AddForce(-forceDirection * force, 4);
			box->AddForce(-forceDirection * force, 7);
			box->AddForce(-forceDirection * force, 8);
		}

		void Tank::ShootSecondary() {

			tankBodyTrans->GetAccumulatedTranformations(&position,&direction);

			TestShotB* shot = new TestShotB(position);
			shotMgr->AddShot(shot);

		}

		Quaternion<float> Tank::GetTurretRotation() {
			return tankBodyTrans->GetRotation() * tankTurretTrans->GetRotation();
		}

		void Tank::SetShotManager(ShotManager* shotMgr) {
			this->shotMgr = shotMgr;
		}
	}
}
