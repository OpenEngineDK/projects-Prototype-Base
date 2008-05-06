// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _RAPIDLASERGUN_H_
#define _RAPIDLASERGUN_H_

#include "IGun.h"

namespace OpenEngine {
  namespace Prototype {
      namespace Weapons {

	  class RapidLaserGun : public IGun {
	  private:
		  float randomness;

    public:
      RapidLaserGun();

      virtual ~RapidLaserGun();

      void ShootGun(ShotPosAndDir posAndDir);

	  bool GunReady();

    };
      }

  } // NS Utils
} // NS OpenEngine

#endif // _DEFAULT_RIGID_BODY_RENDER_NODE_H_
