// Rigid body rendering.
// -------------------------------------------------------------------
// Copyright (C) 2007 (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _GAME_ORGANIZER_H_
#define _GAME_ORGANIZER_H_

namespace OpenEngine {
	namespace Prototype {

		class GameOrganizer {
		protected:
			//! The only existing instance.
			static GameOrganizer* instance;

			GameOrganizer();

		public:
			virtual ~GameOrganizer();

			static GameOrganizer& Instance();
		};

	} // NS Utils
} // NS OpenEngine

#endif // _GAME_ORGANIZER_H_
