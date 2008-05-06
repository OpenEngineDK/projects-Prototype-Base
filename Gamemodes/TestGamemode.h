// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _TESTGAMEMODE_H_
#define _TESTGAMEMODE_H_

#include "IGamemode.h"

namespace OpenEngine {
    namespace Prototype {
        namespace Gamemode {

            using namespace std;
            using namespace OpenEngine::Core;

            class TestGamemode : public IGamemode {
                public:
                    TestGamemode();

                    virtual ~TestGamemode();

                    void Join(ITank* tank);

                    void Quit(ITank* tank);

                    void Kill(ITank* killer, ITank* killee, IGun* gun);

                    void Initialize();
                    void Deinitialize();
                    virtual void Process(const float dt, const float percent);
            };
        }
    } // NS Prototype
} // NS OpenEngine

#endif
