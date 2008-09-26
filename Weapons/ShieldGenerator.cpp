
#include "ShieldGenerator.h"
#include "../GunManager.h"
#include "Shield.h"
#include "../Vehicles/ITank.h"
#include <Logging/Logger.h>

using namespace OpenEngine::Prototype;
using namespace OpenEngine::Prototype::Vehicles;
using namespace OpenEngine::Logging;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {

		ShieldGenerator::ShieldGenerator() {
			lastFired = Time(0);
			delayTime = Time(10000000); // 10000ms
		}

		ShieldGenerator::~ShieldGenerator() {}

		void ShieldGenerator::ShootGun(ShotPosAndDir posAndDir) {
            lastFired = Timer::GetTime();

			Shield* shot = new Shield(this);
			gunMgr->GetTank()->GetShotManager()->AddShot(shot);
		}

		bool ShieldGenerator::GunReady() {
			return (Timer::GetTime() >= lastFired + delayTime);
		}

		Vector<3,float> ShieldGenerator::UpdatePosition() {
			return gunMgr->GetShotPosAndDir(this).first;
		}
            }
	} // NS Utils
} // NS OpenEngine

