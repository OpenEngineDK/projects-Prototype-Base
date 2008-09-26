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

#include "Vehicles/ITank.h"
#include <Core/IModule.h>
#include <map>

namespace OpenEngine {
    namespace Prototype {

        using namespace std;
        using namespace OpenEngine::Core;
        using namespace OpenEngine::Prototype::Vehicles;

        typedef std::map<int,Vehicles::ITank* > TankMap;

        class TankManager : public IModule  {
        private:
            int newID;
            TankMap tankMap;
        public:
            TankManager();

            virtual ~TankManager();

            void AddTank(ITank* tank);
                
            void AddTank(ITank* tank, int ID);

            void RemoveTank(int i);

            ITank* GetTank(int i);


            TankMap GetTankMap();

            void Handle(InitializeEventArg arg);
            void Handle(ProcessEventArg arg);
            void Handle(DeinitializeEventArg arg);

        };
    } // NS Prototype
} // NS OpenEngine

#endif
