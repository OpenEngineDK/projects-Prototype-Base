// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _TEMPLATED_GAMEMODE_H_
#define _TEMPLATED_GAMEMODE_H_

#include "IGamemode.h"

namespace OpenEngine {
	namespace Prototype {
		namespace Gamemode {

			template <class T> class TemplatedGamemode : public IGamemode  {

			protected:
				GamemodeCallbackHandler<T>* cbHandler;

				void SetupCallbackHandler(T* ins) {
					cbHandler = new GamemodeCallbackHandler<T>(ins);
					genericCBHandler = cbHandler;
				}

			public:
				TemplatedGamemode() { 
					cbHandler = NULL;
				};

				virtual ~TemplatedGamemode() {};

				virtual void SetCallback( int (T::*ptr)(), float interval = 0.0, bool looping = false ) {
					if ( cbHandler != NULL ) {
						cbHandler->SetCallback(ptr, interval, looping);
					}
				}

				virtual void StopCallback( int (T::*ptr)() ) {
					if ( cbHandler != NULL ) {
						cbHandler->StopCallback(ptr);
					}
				}
			};
		}
	} // NS Prototype
} // NS OpenEngine

#endif
