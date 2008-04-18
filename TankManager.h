// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _TANKMANAGER_H_
#define _TANKMANAGER_H_

#include "Tank.h"

namespace OpenEngine {
    namespace Prototype {

        using namespace std;
        using namespace OpenEngine::Utils;

        typedef map<int,Tank*> TankMap;

        class TankManager {
            private:
                TankMap tankMap;
            public:
                TankManager();

                virtual ~TankManager();

                void AddTank(Tank* tank);

                void RemoveTank(int i);

                Tank* GetTank(int i);

        };
    } // NS Prototype
} // NS OpenEngine

#endif
