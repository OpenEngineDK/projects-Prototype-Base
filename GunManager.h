// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _GUNMANAGER_H_
#define _GUNMANAGER_H_

#include "IGun.h"
#include <map>
#include <Math/Vector.h>
#include <Math/Quaternion.h>

namespace OpenEngine { 
	namespace Prototype { class ITank; } 
}

//#define PRIMARY 1
//#define SECONDARY 2
//#define TERTIARY 3

namespace OpenEngine {
	namespace Prototype {

		using namespace std;
		using namespace OpenEngine::Math;

		typedef map<int,IGun*> GunMap;
		typedef pair<Vector<3,float>,Quaternion<float>> ShotPosAndDir;

		class GunManager  {
		public:
			typedef enum { PRIMARY_WEAPON, SECONDARY_WEAPON, TERTIARY_WEAPON } WeaponRank;
			typedef map<IGun*,WeaponRank> WeaponRankMap;

		private:
			ITank* tank;
			int newID;
			GunMap gunMap;
			WeaponRankMap rankMap;

		public:	
			GunManager(ITank* tank);

			virtual ~GunManager();

			void AddGun(IGun* gun, WeaponRank rank);

			void RemoveGun(int i);

			IGun* GetGun(int i);

			int GetGunID(IGun* gun);

			GunMap GetGunMap();

			ShotPosAndDir GetShotPosAndDir(IGun* gun);

			void ShootGun(int i);

			ITank* GetTank();

		};
	} // NS Prototype
} // NS OpenEngine

#endif
