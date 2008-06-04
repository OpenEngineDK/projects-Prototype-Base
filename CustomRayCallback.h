/* @(#)CustomRayCallback.h
 */

#ifndef _CUSTOMRAYCALLBACK_H
#define _CUSTOMRAYCALLBACK_H 1

#include <Physics/IRayResultCallback.h>
#include <Math/Vector.h>
#include <Physics/IRigidBody.h>

class CustomRayCallback : public OpenEngine::Physics::IRayResultCallback {
 public:
  CustomRayCallback();

  void AddResult(RayCastResult result);
  
  bool hasHit;
  float fraction;
  OpenEngine::Math::Vector<3,float> normal;
  OpenEngine::Physics::IRigidBody * body;
};

#endif /* _CUSTOMRAYCALLBACK_H */

