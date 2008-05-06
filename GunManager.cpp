
#include "GunManager.h"
#include "Vehicles/ITank.h"

using namespace OpenEngine::Prototype::Vehicles;

namespace OpenEngine {
	namespace Prototype {

		GunManager::GunManager(ITank* tank) {
			this->tank = tank;
			newID = 0;
		}

		GunManager::~GunManager() {
		}

		void GunManager::AddGun(IGun* gun, WeaponRank rank) {
			gun->RegisterGunManager(this);
			gunMap[newID] = gun;
			rankMap[gun] = rank;
			newID++;
		}

		void GunManager::RemoveGun(int i) {
			GunMap::iterator elm = gunMap.find(i);
			if (elm == gunMap.end()) {
				printf("No gun with id: %d!\n", i);
			} else {
				gunMap.erase(i);
			}
		}

		IGun* GunManager::GetGun(int i) {
			GunMap::iterator elm = gunMap.find(i);
			if (elm == gunMap.end()) {
				printf("No gun with id: %d!\n", i);
				return NULL;
			}
			return elm->second;
		}

		int GunManager::GetGunID(IGun* gun) {
			GunMap::iterator iter;
			for (iter =  gunMap.begin(); iter != gunMap.end(); iter++) {
				if ( iter->second == gun ) {
					// Weapon ID found
					return iter->first;
				}
			}
			// ERROR!
			return -1;
		}

		GunMap GunManager::GetGunMap() {
			return gunMap;
		}

		ShotPosAndDir GunManager::GetShotPosAndDir(IGun* gun) {
			ShotPosAndDir res;
			if ( rankMap[gun] == PRIMARY_WEAPON ) {				
				tank->GetTurretGunTransformationNode()->GetAccumulatedTransformations(&res.first,&res.second);									
				res.first = res.first + res.second.RotateVector(Vector<3,float>(20.0, 0.0, 0.0));
			} else if ( rankMap[gun] == SECONDARY_WEAPON ) {
				tank->GetTankTransformationNode()->GetAccumulatedTransformations(&res.first,&res.second);
			}		
			return res;
		}

		void GunManager::ShootGun(int i) {
			IGun* gun = GetGun(i);
			if (gun != NULL) {
				if ( gun->GunReady() ) {
					gun->ShootGun(GetShotPosAndDir(gun));
				}
			}
		}

		ITank* GunManager::GetTank() {
			return tank;
		}

	} // NS Prototype
} // NS OpenEngine

