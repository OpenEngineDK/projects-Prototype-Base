// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _IGAMEMODE_H_
#define _IGAMEMODE_H_

#include "../Vehicles/ITank.h"
#include "../Weapons/IGun.h"
#include <Core/IModule.h>

namespace OpenEngine {
    namespace Prototype {
        namespace Gamemode {

            using namespace std;
            using namespace OpenEngine::Core;
            using namespace OpenEngine::Prototype::Vehicles;

            class IGamemode : public IModule  {
                public:
                    IGamemode() {};

                    virtual ~IGamemode() {};

                    virtual void Join(ITank* tank) {};

                    virtual void Quit(ITank* tank) {};

                    virtual void Kill(ITank* killer, ITank* killee, IGun* gun) {};

                    virtual void Initialize() {};
                    virtual void Deinitialize() {};
                    virtual bool IsTypeOf(const std::type_info& inf) { return false; };
                    virtual void Process(const float dt, const float percent) {};
            };
        }
    } // NS Prototype
} // NS OpenEngine

#endif
