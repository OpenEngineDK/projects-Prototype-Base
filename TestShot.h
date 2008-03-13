// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _TEST_SHOT_NODE_H_
#define _TEST_SHOT_NODE_H_

#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Scene/ISceneNode.h>
#include <Meta/OpenGL.h>
#include <vector>

// forward declarations
namespace OpenEngine { 
	//namespace Geometry  { class Box; } 
	namespace Scene     { class TransformationNode; } 
	namespace Renderers { class IRenderNode; } 
}

using namespace OpenEngine::Math;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Renderers;
using namespace std;

namespace OpenEngine {
	namespace Utils {

                class TestShot : public IRenderNode, public IModule {
		private:
			struct ShotStruct {
				Vector<3,float> from;
				Vector<3,float> to;
				Vector<3,float> color;
				double timeCreated;
			};
			list<ShotStruct*>::iterator shotIter;
			list<ShotStruct*> shots;
			vector<ShotStruct*> toDelete;
			float decayTime;
			float shotSpeed;
			Vector<3,float> gravity;

		public:
			TestShot();

			virtual ~TestShot();
                        
                        void Initialize();
                        void Deinitialize();
                        virtual bool IsTypeOf(const std::type_info& inf);
                        virtual void Process(const float dt, const float percent);

			void Apply(IRenderingView* view);

			void AddShot(Vector<3,float> from, Vector<3,float> to, Vector<3,float> color);

			void SetCrosshair(Vector<3,float> first, Vector<3,float> second, Vector<3,float> third);
		};

	} // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
