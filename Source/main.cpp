#include "./Tortuga.hpp"

using namespace Tortuga;
bool ShouldClose = false;

int main()
{
  Core::Input::NotifyOnWindowClose([]{
    ShouldClose = true;
  });
  Core::Engine::AddSystem<Systems::Rendering>();
  while (!ShouldClose)
  {
    Core::Engine::IterateSystems();
  }
  return EXIT_SUCCESS;
}