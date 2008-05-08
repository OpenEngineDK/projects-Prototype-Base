
#include "TestGamemode.h"

namespace OpenEngine {
    namespace Prototype {
        namespace Gamemode {

			int TestGamemode::OnGameModeInit() {
				printf("TestGamemode::OnGameModeInit()\n");


				Callback<TestGamemode>* cb;
				//call TestCallbackFunction immediately and just once
				cb = new Callback<TestGamemode>(*this,&TestGamemode::TestCallbackFunction);
				AddCallback(cb);

				//call TestCallbackFunction2 after 5 seconds, but just once
				cb = new Callback<TestGamemode>(*this,&TestGamemode::TestCallbackFunction2, 15000);
				AddCallback(cb);

				count = 0;
				//call TestCallbackFunction3 every 2 seconds, 5 times in total
				cb = new Callback<TestGamemode>(*this,&TestGamemode::TestCallbackFunction3, 2000, 5);
				AddCallback(cb);

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
				OnPlayerCommandText(playerid, "I have connected!");
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
				string toWrite = string(tmp) + cmdtext + "\n";
				printf(toWrite.c_str());
				return 1;
			}

			int TestGamemode::TestCallbackFunction() {
				printf("This should be called immediately! - TestGamemode::TestCallbackFunction()\n");
				return 1;
			}

			int TestGamemode::TestCallbackFunction2() {
				printf("This should be called after 15 seconds - TestGamemode::TestCallbackFunction2()\n");
				return 1;
			}

			int TestGamemode::TestCallbackFunction3() {
				count++;
				printf("This has been called %d times - TestGamemode::TestCallbackFunction3()\n",count);
				return 1;
			}
        }
    } // NS Prototype
} // NS OpenEngine

