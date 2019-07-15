#include "Tortuga.h"

using namespace Tortuga;

int main() {
    Core::CreateEngine();
    Core::CreateSystem<Systems::MultiGPURenderingSystem>();

    Core::EngineMainLoop();

    Core::DestroyEngine();
    return 0;
}
