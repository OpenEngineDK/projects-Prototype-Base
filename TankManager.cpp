#include "TankManager.h"
#include <Logging/Logger.h>

using namespace OpenEngine::Logging;

namespace OpenEngine {
    namespace Prototype {

        TankManager::TankManager() {
			newID = 0;
        }

        TankManager::~TankManager() {
        }
        
        void TankManager::AddTank(ITank* tank) {
            tankMap[newID] = tank;
	    	newID++;
        }
        
        void TankManager::AddTank(ITank* tank, int ID) {
            tankMap[ID] = tank;
        }

        void TankManager::RemoveTank(int i) {
            TankMap::iterator elm = tankMap.find(i);
            if (elm == tankMap.end()) {
                printf("No tank with id: %d!\n", i);
            } else {
                tankMap.erase(i);
            }
        }

        ITank* TankManager::GetTank(int i) {
            TankMap::iterator elm = tankMap.find(i);
            if (elm == tankMap.end()) {
                printf("No tank with id: %d!\n", i);
				return NULL;
            }
            return elm->second;
        }

        void TankManager::Handle(InitializeEventArg arg) 
        {
        }
      
        void TankManager::Handle(ProcessEventArg arg)
        {
            float dt = arg.approx / 1000.0;
            TankMap::iterator iter;
            for (iter =  tankMap.begin(); iter != tankMap.end(); iter++) {
                ITank* tank = (*iter).second;
                tank->Process(dt);
            }
        }
      
        void TankManager::Handle(DeinitializeEventArg arg) 
        {
        }

        TankMap TankManager::GetTankMap() {
            return tankMap;
        }

    } // NS Prototype
} // NS OpenEngine

