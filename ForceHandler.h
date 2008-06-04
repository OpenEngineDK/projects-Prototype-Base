/* @(#)ForceHandler.h
 */

#ifndef _FORCEHANDLER_H
#define _FORCEHANDLER_H 1

#include <Devices/IKeyboard.h>
#include <Core/IModule.h>
#include <Physics/DynamicBody.h>
#include <Physics/PhysicsFacade.h>
#include "CustomRayCallback.h"
#include <Renderers/IRenderNode.h>
#include <Renderers/IRenderingView.h>
#include <Math/Vector.h>

class ForceHandler : public OpenEngine::Core::IListener<OpenEngine::Devices::KeyboardEventArg>,
  public OpenEngine::Core::IModule {

 public:

  ForceHandler(OpenEngine::Physics::DynamicBody * body, 
               OpenEngine::Physics::PhysicsFacade * physics);

  void Handle(OpenEngine::Devices::KeyboardEventArg arg);


  void Initialize();

  void Process(const float deltaTime, const float percent);
  void Deinitialize();
  bool IsTypeOf(const std::type_info& inf);

  OpenEngine::Renderers::IRenderNode * GetRenderNode();

 private:

  class RenderForceNode : public OpenEngine::Renderers::IRenderNode {
  public:
    RenderForceNode();
    void Apply(OpenEngine::Renderers::IRenderingView *view);
    OpenEngine::Math::Vector<3,float> rayStart;
    OpenEngine::Math::Vector<3,float> rayEnd;
    double rayTime;
    OpenEngine::Math::Vector<3,float> normal;
  };

  OpenEngine::Physics::DynamicBody * body;
  OpenEngine::Physics::PhysicsFacade * physics;
  CustomRayCallback * callback;
  bool up,down,left,right;
  RenderForceNode rNode;

};

#endif /* _FORCEHANDLER_H */

