
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
			bool deleted = false;
			for ( unsigned int i = 0; i < toDelete.size(); i++ ) {
				shots.remove(toDelete[i]);
				deleted = true;
			}
			if ( deleted ) {
				toDelete.clear();
			}

			for (shotIter =  shots.begin(); shotIter != shots.end(); shotIter++) {
				IShot* shot = (*shotIter);
				shot->Process(dt);
			}
		}

		void ShotManager::Apply(IRenderingView* view) {
			// Render the shots
			for (shotIter =  shots.begin(); shotIter != shots.end(); shotIter++) {
				IShot* shot = (*shotIter);
				shot->Apply(view);
			}	
		}

		void ShotManager::AddShot(IShot* shot) {
			shot->RegisterShotManager(this);
			shots.push_back(shot);
		}

		void ShotManager::DeleteShot(IShot* shot) {
			toDelete.push_back(shot);
		}
	} // NS Utils
} // NS OpenEngine

