
#include "TestGamemode.h"

namespace OpenEngine {
    namespace Prototype {
        namespace Gamemode {

			int TestGamemode::OnGameModeInit() {
				printf("TestGamemode::OnGameModeInit()\n");
				return 1;
			}

			int TestGamemode::OnGameModeExit() {
				printf("TestGamemode::OnGameModeExit()\n");
				return 1;
			}

			int TestGamemode::OnGameModeStart() {
				printf("TestGamemode::OnGameModeStart()\n");
				return 1;
			}

			int TestGamemode::OnGameModeEnd() {
				printf("TestGamemode::OnGameModeEnd()\n");
				return 1;
			}

			int TestGamemode::OnGameLoopProcess(float timeSinceLast) {
				/* 
				* Yeah right xD
				* printf("TestGamemode::OnGameLoopProcess()\n");
				*/
				return 0;
			}

			int TestGamemode::OnPlayerConnect(int playerid) {
				printf("Player with id '%d' connected\n",playerid);
				return 1;
			}

			int TestGamemode::OnPlayerDisconnect(int playerid) {
				printf("Player with id '%d' disconnected\n",playerid);
				return 1;
			}

			int TestGamemode::OnPlayerRequestClass(int playerid, int classid) {
				printf("Player with id '%d' requested class with id '%d'\n",playerid,classid);
				return 1;
			}

			int TestGamemode::OnPlayerSpawn(int playerid) {
				printf("Player with id '%d' spawned\n",playerid);
				return 1;
			}

			int TestGamemode::OnPlayerDeath(int playerid, int killerid, int reason) {
				printf("Player %d killed Player %d with %d",playerid,killerid,reason);
				return 1;
			}

			int TestGamemode::OnPlayerCommandText(int playerid, string cmdtext) {
				char tmp[256];
				sprintf(tmp, "Player %d: ", playerid);
				string toWrite = string(tmp) + cmdtext;
				printf(toWrite.c_str());
				return 1;
			}
        }
    } // NS Prototype
} // NS OpenEngine

