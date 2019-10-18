#include "./Tortuga.hpp"

using namespace Tortuga;
bool ShouldClose = false;

void glfwErrorCallback(int type, const char *message)
{
  Console::Error(message);
}

int main()
{
  //setup engine
  Core::Engine::Create();

  //setup events (can use these anywhere)
  Core::Input::NotifyOnWindowClose([] {
    ShouldClose = true;
  });
  Core::Input::NotifyOnKeyEvent([](Core::Input::KeyCode key, Core::Input::KeyAction event) {
    if (key == Core::Input::KeyCode::Space)
      ShouldClose = true;
  });

  //create a cube with a mesh component
  const auto cube = Core::Engine::CreateEntity();
  const auto mesh = Utils::IO::LoadObjFile("Assets/Models/Cube.obj");
  //Core::Engine::AddComponent<Components::Mesh>(cube, Components::Mesh(mesh));

  //add a rendering system to the engine
  Core::Engine::AddSystem<Systems::Rendering>();

  //main loop
  while (!ShouldClose)
    Core::Engine::IterateSystems();

  //auto destroys everything
  Core::Engine::Destroy();
  return EXIT_SUCCESS;
}