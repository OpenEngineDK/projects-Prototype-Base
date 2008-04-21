
#include "ShieldGenerator.h"
#include "GunManager.h"
#include "Shield.h"
#include "ITank.h"

namespace OpenEngine {
	namespace Prototype {

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
	} // NS Utils
} // NS OpenEngine

