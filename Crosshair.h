// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _CROSSHAIR_H_
#define _CROSSHAIR_H_

#include <Scene/RenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Scene/ISceneNode.h>
#include <Meta/OpenGL.h>

// forward declarations
namespace OpenEngine { 
  //namespace Geometry  { class Box; } 
  namespace Scene     { class TransformationNode;
                        class RenderNode; } 
}

using namespace OpenEngine::Math;
using namespace OpenEngine::Geometry;
using namespace OpenEngine::Scene;
using namespace OpenEngine::Renderers;

namespace OpenEngine {
  namespace Utils {

    class Crosshair : public RenderNode {  
    public:
      Crosshair();
    
      virtual ~Crosshair();

      void Apply(IRenderingView* view);
    };

  } // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
