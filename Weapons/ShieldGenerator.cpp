
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
			lastFired = 0.0;
			delayTime = 10000.0;
		}

		ShieldGenerator::~ShieldGenerator() {}

		void ShieldGenerator::ShootGun(ShotPosAndDir posAndDir) {
      //			lastFired = Timer::GetTime();
      logger.error << "Timing broken in ShieldGenerator" << logger.end;

			Shield* shot = new Shield(this);
			gunMgr->GetTank()->GetShotManager()->AddShot(shot);
		}

		bool ShieldGenerator::GunReady() {
      logger.error << "Timing broken in ShieldGenerator" << logger.end;
      return false;
      
			//return (Timer::GetTime() >= lastFired + delayTime);
		}

		Vector<3,float> ShieldGenerator::UpdatePosition() {
			return gunMgr->GetShotPosAndDir(this).first;
		}
            }
	} // NS Utils
} // NS OpenEngine

