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

			tShot = new TestShot();

			tankBodyTrans = new TransformationNode();
			tankBodyTrans->AddNode(tankBody);

                        cameraRotateTrans = new TransformationNode();
                        cameraPivotTrans = new TransformationNode();
                        cameraRotateTrans->AddNode(cameraPivotTrans);

			tankTurretTrans = new TransformationNode();
			tankTurretGunTrans = new TransformationNode();	 //
			tankTurretGunTrans->Move(0.0, 2.0, 0.0);
                        tankTurretTrans->AddNode(tankTurretGunTrans); //
			tankTurretGunTrans->AddNode(tankGun);
			tankTurretTrans->AddNode(tankTurret);
			tankBodyTrans->AddNode(tankTurretTrans); //
                        tankBodyTrans->AddNode(cameraRotateTrans);
			tankTurretTrans->Move(0.0, 7.0, 0.0);	

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
			//position = tankBodyTrans->GetPosition() + tankBodyTrans->GetRotation().RotateVector(tankTurretTrans->GetPosition() + tankTurretTrans->GetRotation().RotateVector(tankTurretGunTrans->GetPosition()));
			//direction = tankBodyTrans->GetRotation() * tankTurretTrans->GetRotation() * tankTurretGunTrans->GetRotation();
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

		void Tank::ShootCannon() {
			position = tankBodyTrans->GetPosition() + tankBodyTrans->GetRotation().RotateVector(tankTurretTrans->GetPosition() + tankTurretTrans->GetRotation().RotateVector(tankTurretGunTrans->GetPosition()));
			direction = tankBodyTrans->GetRotation() * tankTurretTrans->GetRotation() * tankTurretGunTrans->GetRotation();

			Vector<3,float> shotDir = Vector<3,float>(40.0, 0.0, 0.0);
			Vector<3,float> posOfCannonBarrel = Vector<3,float>(20.0, 0.0, 0.0);
			
			shotDir = direction.RotateVector(shotDir) + position;
			posOfCannonBarrel = direction.RotateVector(posOfCannonBarrel) + position;

			Vector<3,float> c = Vector<3,float>((float)std::rand()/RAND_MAX, (float)std::rand()/RAND_MAX, (float)std::rand()/RAND_MAX);
			tShot->AddShot(posOfCannonBarrel,shotDir,c);

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

		TestShot* Tank::GetTestShot() {
			return tShot;
		}

		Quaternion<float> Tank::GetTurretRotation() {
			return tankBodyTrans->GetRotation() * tankTurretTrans->GetRotation();
		}
	}
}
