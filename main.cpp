// main
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// OpenEngine stuff
#include <Meta/Config.h>

#include <Core/Engine.h>
#include <Logging/Logger.h>
#include <Logging/StreamLogger.h>

// Game factory
#include "GameFactory.h"

// name spaces that we will be using.
// this combined with the above imports is almost the same as
// fx. import OpenEngine.Logging.*; in Java.
using namespace OpenEngine::Logging;
using namespace OpenEngine::Core;

/**
 * Main method for the first quarter project of CGD.
 * Corresponds to the
 *   public static void main(String args[])
 * method in Java.
 */
int main(int argc, char** argv) {
    // Setup logging facilities.
    Logger::AddLogger(new StreamLogger(&std::cout));

    // Print usage info.
    logger.info << "========= Beaten Traxx Prototype =========" << logger.end;

//     // Start the engine.
//     IGameEngine& engine = GameEngine::Instance();
//     engine.Start(new GameFactory());

    Engine * engine = new Engine();
    GameFactory * gameFactory = new GameFactory();
    gameFactory->SetupEngine(*engine);
    engine->Start();
    // Return when the engine stops.
    return EXIT_SUCCESS;
}
