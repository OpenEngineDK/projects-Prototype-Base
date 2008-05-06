
#include "ShieldGenerator.h"
#include "../GunManager.h"
#include "Shield.h"
#include "../Vehicles/ITank.h"

using namespace OpenEngine::Prototype;
using namespace OpenEngine::Prototype::Vehicles;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {

		ShieldGenerator::ShieldGenerator() {
			lastFired = 0.0;
			delayTime = 10000.0;
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

