#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Core::Engine::AddSystem<Systems::Rendering>();
  while (true)
  {
    Core::Engine::IterateSystems();
  }
  return EXIT_SUCCESS;
}