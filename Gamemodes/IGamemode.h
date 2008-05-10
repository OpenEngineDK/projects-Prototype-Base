// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _IGAMEMODE_H_
#define _IGAMEMODE_H_

#include <Core/IModule.h>
#include "Utils/GenericGamemodeCallbackHandler.h"
#include "Utils/GamemodeCallbackHandler.h"
#include <string>

namespace OpenEngine {
	namespace Prototype {
		namespace Gamemode {

			using namespace std;
			using namespace OpenEngine::Core;

			class IGamemode : public IModule  {
			protected:
				GenericGamemodeCallbackHandler* genericCBHandler;

			public:
				IGamemode() { genericCBHandler = NULL; };

				virtual ~IGamemode() {};

				bool IsTypeOf(const std::type_info& inf) { return false; };

				void Initialize() { OnGameModeInit(); };
				void Deinitialize() { OnGameModeExit(); };                    
				void Process(const float dt, const float percent) { 
					if ( genericCBHandler != NULL ) {
						genericCBHandler->ProcessCallbacks(dt);
					}
					OnGameLoopProcess(dt);
				};

				// Implemented for each gamemode
				virtual int OnGameModeInit() = 0;
				virtual int OnGameModeExit() = 0;
				virtual int OnGameModeStart() = 0;
				virtual int OnGameModeEnd() = 0;
				virtual int OnGameLoopProcess(float timeSinceLast) = 0;
				virtual int OnPlayerConnect(int playerid) = 0;
				virtual int OnPlayerDisconnect(int playerid) = 0;
				virtual int OnPlayerRequestClass(int playerid, int classid) = 0;
				virtual int OnPlayerSpawn(int playerid) = 0;
				virtual int OnPlayerDeath(int playerid, int killerid, int reason) = 0;
				virtual int OnPlayerEnterZone(int playerid, int zoneid) = 0;
				virtual int OnPlayerLeaveZone(int playerid, int zoneid) = 0;
				virtual int OnPlayerCommandText(int playerid, string cmdtext) = 0;
			};
		}
	} // NS Prototype
} // NS OpenEngine

#endif
