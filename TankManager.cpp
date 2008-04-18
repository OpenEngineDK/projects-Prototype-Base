
#include "TankManager.h"

namespace OpenEngine {
    namespace Prototype {

        TankManager::TankManager() {
		newID = 0;
        }

        TankManager::~TankManager() {
        }

        void TankManager::AddTank(Tank* tank) {
            tankMap[newID] = tank;
	    newID++;
        }

        void TankManager::RemoveTank(int i) {
            TankMap::iterator elm = tankMap.find(i);
            if (elm == tankMap.end()) {
                printf("No tank with id: %d", i);
            } else {
                tankMap.erase(i);
            }
        }

        Tank* TankManager::GetTank(int i) {
            TankMap::iterator elm = tankMap.find(i);
            if (elm == tankMap.end()) {
                printf("No tank with id: %d", i);
            }
            return elm->second;
        }

	void TankManager::Initialize() {
	}
	
	void TankManager::Deinitialize() {
	}
	
	bool TankManager::IsTypeOf(const std::type_info& inf) {
		return false;
	}
	
	void TankManager::Process(const float dt, const float percent) {
		TankMap::iterator iter;
		for (iter =  tankMap.begin(); iter != tankMap.end(); iter++) {
			Tank* tank = (*iter).second;
			tank->Process(dt,percent);
		}
	}

    } // NS Prototype
} // NS OpenEngine

