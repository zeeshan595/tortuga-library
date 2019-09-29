#include "./Tortuga.hpp"

using namespace Tortuga;
bool ShouldClose = false;

int main()
{
  Core::Engine::Create();
  Core::Input::NotifyOnWindowClose([] {
    ShouldClose = true;
  });
  Core::Engine::AddSystem<Systems::Rendering>();
  while (!ShouldClose)
  {
    Core::Engine::IterateSystems();
  }
  Core::Engine::Destroy();
  return EXIT_SUCCESS;
}