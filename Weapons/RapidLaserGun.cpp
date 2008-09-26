
#include "RapidLaserGun.h"
#include "../GunManager.h"
#include "RapidLaserShot.h"
#include "../Vehicles/ITank.h"
#include <Logging/Logger.h>

using namespace OpenEngine::Prototype;
using namespace OpenEngine::Prototype::Vehicles;
using namespace OpenEngine::Logging;


namespace OpenEngine {
	namespace Prototype {
        namespace Weapons {

            RapidLaserGun::RapidLaserGun() {
                lastFired = Time(0,0);
                delayTime = Time(10000); // 10ms

                randomness = 0.04;
            }

            RapidLaserGun::~RapidLaserGun() {}

            void RapidLaserGun::ShootGun(ShotPosAndDir posAndDir) {
                lastFired = Timer::GetTime();

                Vector<3,float> shotPos = posAndDir.first;
                Quaternion<float> shotDir = posAndDir.second;
                shotDir *= Quaternion<float>( ((float)std::rand()/RAND_MAX - 0.5) * randomness, ((float)std::rand()/RAND_MAX - 0.5) * randomness, ((float)std::rand()/RAND_MAX - 0.5) * randomness );

                Vector<3,float> shotLength = Vector<3,float>(10.0, 0.0, 0.0);
			
                shotLength = shotDir.RotateVector(shotLength) + shotPos;

                RapidLaserShot* shot = new RapidLaserShot(shotPos,shotLength);
                gunMgr->GetTank()->GetShotManager()->AddShot(shot);

                /*
                  OpenEngine::Physics::RigidBody* box = gunMgr->GetTank()->GetRigidBox();
			
                  if( box == NULL ) return;
                  float force = 70.0;
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

            bool RapidLaserGun::GunReady() {
                return (Timer::GetTime() >= lastFired + delayTime);
            }
        }

	} // NS Utils
} // NS OpenEngine

