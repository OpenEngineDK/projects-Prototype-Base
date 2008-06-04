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

namespace OpenEngine {
    namespace Prototype {

        using namespace std;
	using namespace OpenEngine::Core;
        using namespace OpenEngine::Prototype::Vehicles;

        typedef map<int,ITank*> TankMap;

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

		void Initialize();
		void Deinitialize();
		virtual bool IsTypeOf(const std::type_info& inf);
		virtual void Process(const float dt, const float percent);

		TankMap GetTankMap();

        };
    } // NS Prototype
} // NS OpenEngine

#endif
