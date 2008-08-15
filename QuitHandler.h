#ifndef _QUIT_HANDLER_H_
#define _QUIT_HANDLER_H_

#include <Core/Engine.h>
#include <Devices/Symbols.h>
#include <Devices/IKeyboard.h>

using namespace OpenEngine::Devices;

/**
 * Quit handler.
 *
 * This is an example of a keyboard listener.
 * Every time the a key is released a key (UP) event is sent to the
 * handle method. If the key symbol is escape it quits the game.
 */
class QuitHandler : public IListener<KeyboardEventArg> {
 public:

  QuitHandler(OpenEngine::Core::Engine & engine) :
    engine(engine)
    {} 
      
    void Handle(KeyboardEventArg arg) {
      if (arg.sym == KEY_ESCAPE) {
        engine.Stop();
      }
    }
        
    private:
    
    OpenEngine::Core::Engine & engine;
};
 
#endif
