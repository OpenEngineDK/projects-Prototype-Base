// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _I_SHOT_H_
#define _I_SHOT_H_

#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Scene/ISceneNode.h>
#include <Meta/OpenGL.h>

// forward declarations
namespace OpenEngine { 
	//namespace Geometry  { class Box; } 
	namespace Scene     { class TransformationNode; } 
	namespace Renderers { class IRenderNode; }
	namespace Prototype { class ShotManager; } 
}

using namespace OpenEngine::Math;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Renderers;
using namespace std;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {

        class IShot : public IRenderNode {
		protected:
			ShotManager* shotMgr;

		public:
			IShot() {};
			virtual ~IShot() {};
            virtual void Process(const float timeSinceLast) {};

			void Apply(IRenderingView* view) {};

			virtual void Destroy() = 0;

			virtual void RegisterShotManager(ShotManager* shotMgr) {
				this->shotMgr = shotMgr;
			}
		};
            } // NS Weapons
	} // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
