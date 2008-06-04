#include "CustomRayCallback.h"

#include <Logging/Logger.h>

using namespace OpenEngine::Logging;

CustomRayCallback::CustomRayCallback() :
  hasHit(false)
{}
void CustomRayCallback::AddResult(RayCastResult result) {
  if(result.fraction < fraction) {
    hasHit = true;
    body = result.bodyHit;
  
    fraction = result.fraction;
    normal = result.normal;
  }
  logger.info << result.bodyHit->GetName() << logger.end;
}
