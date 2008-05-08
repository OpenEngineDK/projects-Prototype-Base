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
#include <string>
#include <list>

namespace OpenEngine {
	namespace Prototype {
		namespace Gamemode {

			using namespace std;
			using namespace OpenEngine::Core;

			class IGamemode : public IModule  {
			protected:
				/**
				* Generic callback
				* Used so templated callbacks can easily be stored in a single structure
				* and invoked from the IGamemode class.
				* @class GenericCallback
				*/
				class GenericCallback {
				public:
					float interval;
					float elapsed;
					int repetitions;

					GenericCallback() { interval = 0.0; elapsed = 0.0; repetitions = 0; };
					virtual void Execute() {};
				};

				/**
				* Templated callback
				* @class Callback
				* @see GenericCallback
				*/
				template <class T> class Callback : public GenericCallback {
				#define CALLBACK_LOOP -1
				protected:
					//! reference to handler instance
					T& instance;
					//! handler method
					int (T::*memberFunc)();
				public:
					/**
					* Usage:
					* \code
					* \\ create a listener wrapper
					* Callback<TestGamemode>* cb = new Callback<TestGamemode>(*this,&TestGamemode::OnGameModeStart, 10000);
					* \\ now add it to the list of callbacks
					* AddCallback(cb);
					* \\ it will now be called at the interval specified for the specified number of times.
					* \endcode
					*
					* @param ins Reference to handler object of type \a T
					* @param ptr Address to handler method
					* @param interval Time interval between invokes
					* @param repetitions Number of times the callback should be called, CALLBACK_LOOP for loop.
					*/
					Callback(T& ins, int (T::*ptr)(), float interval = 0.0, int repetitions = 0 ): instance(ins) {
						this->interval = interval;
						elapsed = 0.0;
						this->repetitions = repetitions;						
						memberFunc = ptr;
					};
					virtual void Execute() {
						(instance.*memberFunc)();
					};
				};

				list<GenericCallback* > callbacks;
				list<GenericCallback* > callbacksToDelete;
				list<GenericCallback* >::iterator cbIter;

			public:
				IGamemode() {};

				virtual ~IGamemode() {};

				bool IsTypeOf(const std::type_info& inf) { return false; };

				void Initialize() { OnGameModeInit(); };
				void Deinitialize() { OnGameModeExit(); };                    
				void Process(const float dt, const float percent) { 
					for (cbIter =  callbacks.begin(); cbIter != callbacks.end(); cbIter++) {
						(*cbIter)->elapsed += dt;
						if ( (*cbIter)->elapsed > (*cbIter)->interval ) {
							(*cbIter)->elapsed -= (*cbIter)->interval;
							(*cbIter)->Execute();
							if ( (*cbIter)->repetitions > 0 ) {
								(*cbIter)->repetitions--;
							}
							if ( (*cbIter)->repetitions == 0 ) {
								callbacksToDelete.push_back(*cbIter);
							}
						}
					}
					for (cbIter =  callbacksToDelete.begin(); cbIter != callbacksToDelete.end(); cbIter++) {
						RemoveCallback((*cbIter));
					}
					callbacksToDelete.clear();
					OnGameLoopProcess(dt);
				};

				void AddCallback(GenericCallback* cb) { callbacks.push_back(cb); };
				void RemoveCallback(GenericCallback* cb) { callbacks.remove(cb); };

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
				virtual int OnPlayerCommandText(int playerid, string cmdtext) = 0;
			};
		}
	} // NS Prototype
} // NS OpenEngine

#endif
