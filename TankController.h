// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _TANKCONTROLLER_H_
#define _TANKCONTROLLER_H_

#include "TankManager.h"
#include "ClassicMovementHandler.h"
#include <Display/FollowCamera.h>
#include "Crosshair.h"

namespace OpenEngine {
  namespace Prototype {

    using namespace OpenEngine::Utils;

    class TankController {
    private:
      TankManager* tankMgr;
      ClassicMovementHandler* classicMovement;
      FollowCamera* camera;
      ISceneNode* crosshairNode;

    public:
      TankController(TankManager* tankMgr, ClassicMovementHandler* movementHandler, FollowCamera* camera);

      virtual ~TankController();

      void SetPlayerTank(int i);

    };

  } // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
