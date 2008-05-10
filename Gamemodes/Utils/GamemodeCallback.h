// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _GAMEMODE_CALLBACK_H_
#define _GAMEMODE_CALLBACK_H_

#include "GenericGamemodeCallback.h"

namespace OpenEngine {
	namespace Prototype {
		namespace Gamemode {

			/**
			* Templated callback
			* @class Callback
			* @see GenericCallback
			*/
			template <class T> class GamemodeCallback : public GenericGamemodeCallback {
			public:
				//! reference to handler instance
				T& instance;
				//! handler method
				int (T::*memberFunc)();

				/**
				* Usage:
				* @param ins Reference to handler object of type \a T
				* @param ptr Address to handler method
				* @param interval Time interval between invokes
				*/
				GamemodeCallback(T& ins, int (T::*ptr)(), float interval = 0.0, bool looping = false ) : instance(ins) {
					this->interval = interval;
					elapsed = 0.0;					
					memberFunc = ptr;
					this->looping = looping;
				};

				virtual ~GamemodeCallback() {};

				virtual void Execute() {
					(instance.*memberFunc)();
				};
			};
		}
	} // NS Prototype
} // NS OpenEngine

#endif
