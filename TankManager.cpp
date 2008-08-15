
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
        logger.error << "Timig broken in Tankmanager" << logger.end;
        Process(1,1);
      }
      
      void TankManager::Handle(DeinitializeEventArg arg) 
      {
      }


	bool TankManager::IsTypeOf(const std::type_info& inf) {
		return false;
	}
	
	void TankManager::Process(const float dt, const float percent) {
		TankMap::iterator iter;
		for (iter =  tankMap.begin(); iter != tankMap.end(); iter++) {
			ITank* tank = (*iter).second;
			tank->Process(dt);
		}
	}

	TankMap TankManager::GetTankMap() {
		return tankMap;
	}

    } // NS Prototype
} // NS OpenEngine

