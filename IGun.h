// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _IGUN_H_
#define _IGUN_H_

#include <Utils/Timer.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>

namespace OpenEngine { 
	namespace Prototype { class GunManager; } 
}

namespace OpenEngine {
	namespace Prototype {

		using namespace OpenEngine::Utils;
		using namespace OpenEngine::Math;

		typedef std::pair<Vector<3,float>,Quaternion<float>> ShotPosAndDir;

		class IGun {
		protected:
			double lastFired;
			double delayTime;
			GunManager* gunMgr;

		public:
			IGun() {};

			virtual ~IGun() {};

			void RegisterGunManager(GunManager* gunMgr) {
				this->gunMgr = gunMgr;
			}

			virtual void ShootGun(ShotPosAndDir posAndDir) {};

			
			virtual bool GunReady() = 0;

		};

	} // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
