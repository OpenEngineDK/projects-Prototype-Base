// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _GAMEMODE_CALLBACK_HANDLER_H_
#define _GAMEMODE_CALLBACK_HANDLER_H_

#include "GenericGamemodeCallbackHandler.h"
#include "GamemodeCallback.h"

namespace OpenEngine {
    namespace Prototype {
        namespace Gamemode {

			template <class T> class GamemodeCallbackHandler : public GenericGamemodeCallbackHandler {
				protected:
				//! reference to handler instance
				T* instance;

                public:
					GamemodeCallbackHandler(T* ins) {
						this->instance = ins;
					};

					virtual ~GamemodeCallbackHandler() {};

					virtual void SetCallback( int (T::*ptr)(), float interval = 0.0, GenericGamemodeCallback::LoopFlag loopFlag = GenericGamemodeCallback::CALLBACK_ONCE ) {
						GenericGamemodeCallback* callback = new GamemodeCallback<T>(*instance, ptr, interval, loopFlag);
						StopCallback(ptr);
						AddCallback(callback);
					};

					virtual void StopCallback( int (T::*ptr)() ) {
						GenericGamemodeCallback* cb = GetCallback(ptr);
						if ( cb != NULL ) {
							SafeRemoveCallback(cb);
						}
					}

					virtual GenericGamemodeCallback* GetCallback( int (T::*ptr)() ) {
						GenericGamemodeCallback* ret = NULL;
						list<GenericGamemodeCallback* >::iterator iter;
						for (iter =  callbacks.begin(); iter != callbacks.end(); iter++) {
							GamemodeCallback<T>* cb = dynamic_cast<GamemodeCallback<T>* >(*iter);
							if ( cb != NULL ) {
								if ( cb->memberFunc == ptr ) {
									ret = cb;
								}
							}							
						}
						return ret;
					}
            };
        }
    } // NS Prototype
} // NS OpenEngine

#endif
