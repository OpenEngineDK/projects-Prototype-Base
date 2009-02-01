// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _LASER_SHOT_H_
#define _LASER_SHOT_H_

#include "IShot.h"
#include <Scene/RenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Scene/ISceneNode.h>
#include <Meta/OpenGL.h>

// forward declarations
namespace OpenEngine { 
	//namespace Geometry  { class Box; } 
	namespace Scene     { 
        class TransformationNode;
        class RenderNode; 
    } 
}

using namespace OpenEngine::Math;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Renderers;
using namespace std;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {
                    
                class LaserShot : public IShot {
		private:
			double lifeTime;
			double decayTime;
			float shotSpeed;
			Vector<3,float> from;
			Vector<3,float> to;
			Vector<3,float> color;

		public:			
			LaserShot(Vector<3,float> from, Vector<3,float> to);

			virtual ~LaserShot();
            virtual void Process(const float timeSinceLast);

			void Apply(IRenderingView* view);

			void Destroy();
		};
            }
	} // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
