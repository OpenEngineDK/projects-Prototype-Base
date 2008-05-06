
#include "TestGamemode.h"

namespace OpenEngine {
    namespace Prototype {
        namespace Gamemode {
            TestGamemode::TestGamemode() {
            }

            TestGamemode::~TestGamemode() {
            }

            void TestGamemode::Join(ITank* tank) {
                printf("Joined: %p\n",tank);
            }

            void TestGamemode::Quit(ITank* tank) {
                printf("Quit: %p\n",tank);
            }

            void TestGamemode::Kill(ITank* killer, ITank* killee, IGun* gun) {
                printf("%p killed %p with %p\n",killer,killee,gun);
            }

            void TestGamemode::Initialize() {
            }

            void TestGamemode::Deinitialize() {
            }

            void TestGamemode::Process(const float dt, const float percent) {
            }
        }
    } // NS Prototype
} // NS OpenEngine

