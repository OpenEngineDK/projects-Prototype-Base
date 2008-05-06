
#include "LandmineLayer.h"
#include "../GunManager.h"
#include "Landmine.h"
#include "../Vehicles/ITank.h"

using namespace OpenEngine::Prototype;
using namespace OpenEngine::Prototype::Vehicles;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {

		LandmineLayer::LandmineLayer() {
			lastFired = 0.0;
			delayTime = 1000.0;
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

