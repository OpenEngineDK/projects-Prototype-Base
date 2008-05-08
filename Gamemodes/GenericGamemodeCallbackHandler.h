// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _GENERIC_GAMEMODE_CALLBACK_HANDLER_H_
#define _GENERIC_GAMEMODE_CALLBACK_HANDLER_H_

#include "GenericGamemodeCallback.h"
#include <list>

namespace OpenEngine {
	namespace Prototype {
		namespace Gamemode {

			using namespace std;

			class GenericGamemodeCallbackHandler {
			protected:
				list<GenericGamemodeCallback* > callbacks;
				list<GenericGamemodeCallback* > callbacksToDelete;
				list<GenericGamemodeCallback* >::iterator cbIter;

			public:
				GenericGamemodeCallbackHandler() {};

				virtual ~GenericGamemodeCallbackHandler() {};

				virtual void AddCallback(GenericGamemodeCallback* callback) {
					callbacks.push_back(callback);
				};

				virtual void SafeRemoveCallback(GenericGamemodeCallback* callback) { 
					callbacksToDelete.push_back(callback);
				};

				virtual void RemoveCallback(GenericGamemodeCallback* callback) { 
					callbacks.remove(callback);
				};

				virtual void ProcessCallbacks(float timeSinceLast) {
					if ( callbacksToDelete.size() > 0 ) {
						for (cbIter =  callbacksToDelete.begin(); cbIter != callbacksToDelete.end(); cbIter++) {
							RemoveCallback((*cbIter));
						}
					}
					callbacksToDelete.clear();

					for (cbIter =  callbacks.begin(); cbIter != callbacks.end(); cbIter++) {
						(*cbIter)->elapsed += timeSinceLast;
						if ( (*cbIter)->elapsed > (*cbIter)->interval ) {
							(*cbIter)->elapsed -= (*cbIter)->interval;
							(*cbIter)->Execute();
							if ( (*cbIter)->loopFlag == GenericGamemodeCallback::CALLBACK_ONCE ) {
								callbacksToDelete.push_back(*cbIter);
							}
						}
					}
				};
			};
		}
	} // NS Prototype
} // NS OpenEngine

#endif
