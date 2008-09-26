
#include "LandmineLayer.h"
#include "../GunManager.h"
#include "Landmine.h"
#include "../Vehicles/ITank.h"
#include <Logging/Logger.h>

using namespace OpenEngine::Prototype;
using namespace OpenEngine::Prototype::Vehicles;
using namespace OpenEngine::Logging;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {

		LandmineLayer::LandmineLayer() {
			lastFired = Time(0);
			delayTime = Time(1000000); //1000ms
		}

		LandmineLayer::~LandmineLayer() {}

		void LandmineLayer::ShootGun(ShotPosAndDir posAndDir) {
            lastFired = Timer::GetTime();

			Landmine* shot = new Landmine(posAndDir.first);
			gunMgr->GetTank()->GetShotManager()->AddShot(shot);
		}

		bool LandmineLayer::GunReady() {
            return (Timer::GetTime() >= lastFired + delayTime);
		}
            }
	} // NS Utils
} // NS OpenEngine

