
#include "LightningGun.h"
#include "../GunManager.h"
#include "LightningShot.h"
#include "../Vehicles/ITank.h"
#include <Logging/Logger.h>

using namespace OpenEngine::Prototype;
using namespace OpenEngine::Prototype::Vehicles;
using namespace OpenEngine::Logging;

namespace OpenEngine {
	namespace Prototype {
        namespace Weapons {
            LightningGun::LightningGun() {
                lastFired = Time(0);
                delayTime = Time(20000); // 20 ms
            }

            LightningGun::~LightningGun() {}

            void LightningGun::ShootGun(ShotPosAndDir posAndDir) {
                lastFired = Timer::GetTime();

                Vector<3,float> shotPos = posAndDir.first;
                Quaternion<float> shotDir = posAndDir.second; 

                Vector<3,float> shotLength = Vector<3,float>(20.0, 0.0, 0.0);
			
                shotLength = shotDir.RotateVector(shotLength) + shotPos;

                LightningShot* shot = new LightningShot(shotPos,shotLength);
                gunMgr->GetTank()->GetShotManager()->AddShot(shot);

                /*
                  OpenEngine::Physics::RigidBody* box = gunMgr->GetTank()->GetRigidBox();

			
                  if( box == NULL ) return;
                  float force = 75.0;
                  Vector<3,float> forceDirection = (shotPos - shotLength).GetNormalize();
                  box->AddForce(forceDirection * force, 1);
                  box->AddForce(forceDirection * force, 2);
                  box->AddForce(forceDirection * force, 5);
                  box->AddForce(forceDirection * force, 6);

                  box->AddForce(-forceDirection * force, 3);
                  box->AddForce(-forceDirection * force, 4);
                  box->AddForce(-forceDirection * force, 7);
                  box->AddForce(-forceDirection * force, 8);			
                */
            }

            bool LightningGun::GunReady() {
                return (Timer::GetTime() >= lastFired + delayTime);
            }
        }

	} // NS Utils
} // NS OpenEngine

