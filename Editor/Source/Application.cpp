#include <Tortuga.h>

using namespace Tortuga;

const uint32_t WINDOW_WIDTH = 1920;
const uint32_t WINDOW_HEIGHT = 1080;

int main(int argc, char **argv) {
  Core::CreateEngine();
  Core::CreateSystem<Systems::MultiGPURenderingSystem>();
  Core::EngineMainLoop();
  Core::DestroyEngine();
  return 0;
}
