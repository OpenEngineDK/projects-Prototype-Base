
#include "ShotManager.h"
#include <Meta/OpenGL.h>
#include <Utils/Timer.h>
#include "TankManager.h"

using namespace OpenEngine::Renderers;
using namespace OpenEngine::Utils;
//using namespace OpenEngine::Renderers::OpenGL;

namespace OpenEngine {
	namespace Prototype {
		ShotManager::ShotManager() {
		}

		ShotManager::~ShotManager() {}
                void ShotManager::Initialize() {
                }
                void ShotManager::Deinitialize() {
                }
                bool ShotManager::IsTypeOf(const std::type_info& inf) {
                    return false;
                }
                void ShotManager::Process(const float dt, const float percent) {
			double currentTime = Timer::GetTime();
			for (shotIter =  shots.begin(); shotIter != shots.end(); shotIter++) {
				TestShot* shot = (*shotIter);
				shot->Process(dt,percent);
				if ( shot->timeCreated + shot->decayTime < currentTime ) {
					toDelete.push_back(shot);
                        	}
			}
			for ( unsigned int i = 0; i < toDelete.size(); i++ ) {
				shots.remove(toDelete[i]);
			}
			toDelete.clear();
                }

		void ShotManager::Apply(IRenderingView* view) {
			// Render the shots
			for (shotIter =  shots.begin(); shotIter != shots.end(); shotIter++) {
				TestShot* shot = (*shotIter);
				shot->Apply(view);
			}	
		}

		void ShotManager::AddShot(TestShot* shot) {
			shots.push_back(shot);
		}
	} // NS Utils
} // NS OpenEngine

