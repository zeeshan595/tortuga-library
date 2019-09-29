#include "./Tortuga.hpp"

using namespace Tortuga;
bool ShouldClose = false;

int main()
{
  //setup engine
  Core::Engine::Create();

  //setup events (can use these anywhere)
  Core::Input::NotifyOnWindowClose([] {
    ShouldClose = true;
  });
  Core::Input::NotifyOnKeyEvent([](const auto key, const auto event) {
    if (key == Core::Input::KeyCode::Space)
      ShouldClose = true;
  });

  //create a cube with a mesh component
  const auto cube = Core::Engine::CreateEntity();
  const auto obj = Utils::IO::LoadObjFile("Assets/Models/Cube.obj");

  Core::Engine::AddSystem<Systems::Rendering>();

  //main loop
  while (!ShouldClose)
    Core::Engine::IterateSystems();

  //auto destroys everything
  Core::Engine::Destroy();
  return EXIT_SUCCESS;
}