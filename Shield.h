// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _SHIELD_H_
#define _SHIELD_H_

#include "IShot.h"
#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Scene/ISceneNode.h>
#include <Meta/OpenGL.h>

// forward declarations
namespace OpenEngine { 
	//namespace Geometry  { class Box; } 
	namespace Scene     { class TransformationNode; } 
	namespace Renderers { class IRenderNode; } 
	namespace Prototype { class ShieldGenerator; } 
}

using namespace OpenEngine::Math;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Renderers;
using namespace std;

namespace OpenEngine {
	namespace Prototype {

                class Shield : public IShot {
		private:
			double lifeTime;
			double decayTime;
			float shotSpeed;
			Vector<3,float> pos;
			Vector<3,float> color;
			float rotation;
			ShieldGenerator* generator;

		public:
			Shield(ShieldGenerator* generator);

			virtual ~Shield();
            void Process(const float timeSinceLast);

			void Apply(IRenderingView* view);

			void Destroy();
		};
	} // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
