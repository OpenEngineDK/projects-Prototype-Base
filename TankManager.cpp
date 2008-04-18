
#include "TankManager.h"

namespace OpenEngine {
    namespace Prototype {

        TankManager::TankManager() {
        }

        TankManager::~TankManager() {
        }

        void TankManager::AddTank(Tank* tank) {
            tankMap[0] = tank;
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

    } // NS Prototype
} // NS OpenEngine

