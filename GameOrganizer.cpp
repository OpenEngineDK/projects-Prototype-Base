
#include <cstdlib> // For NULL.. Fix plix!
#include "GameOrganizer.h"

namespace OpenEngine {
	namespace Prototype {

		GameOrganizer* GameOrganizer::instance = NULL;

		GameOrganizer::GameOrganizer() {
		}

		GameOrganizer::~GameOrganizer() {
			instance = NULL;
		}

		GameOrganizer& GameOrganizer::Instance() {
			if (instance == NULL)
				instance = new GameOrganizer();
			return *instance;
		}

	} // NS Prototype
} // NS OpenEngine

