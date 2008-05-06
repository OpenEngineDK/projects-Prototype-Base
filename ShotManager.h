// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _SHOT_MANAGER_H_
#define _SHOT_MANAGER_H_

#include <Scene/ISceneNode.h>
#include <Meta/OpenGL.h>
#include <vector>

#include "Weapons/IShot.h"

using namespace OpenEngine::Math;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace std;
using namespace OpenEngine::Prototype::Weapons;

namespace OpenEngine {
	namespace Prototype {

                class ShotManager : public IRenderNode, public IModule {
		private:
			list<IShot*>::iterator shotIter;
			list<IShot*> shots;
			vector<IShot*> toDelete;

		public:
			ShotManager();

			virtual ~ShotManager();

                        void Initialize();
                        void Deinitialize();
                        virtual bool IsTypeOf(const std::type_info& inf);
                        virtual void Process(const float dt, const float percent);

			void Apply(IRenderingView* view);

			void AddShot(IShot* shot);
			void DeleteShot(IShot* shot);
		};

	} // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
