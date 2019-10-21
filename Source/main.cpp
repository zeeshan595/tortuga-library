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
  Core::Input::NotifyOnKeyEvent([](Core::Input::KeyCode key, Core::Input::KeyAction event) {
    if (key == Core::Input::KeyCode::Space)
      ShouldClose = true;
    if (key == Core::Input::A)
    {
      if (event == Core::Input::Pressed)
        Console::Info("Pressed");
      else if (event == Core::Input::Released)
        Console::Info("Released");
      else if (event == Core::Input::HeldDown)
        Console::Info("Held Down");
    }
  });
  Core::Input::NotifyOnMouseEvent([](double x, double y) {
    //Console::Info("Mouse Position: {0}, {1}", Console::Arguments() << x << y);
  });
  Core::Input::NotifyOnWindowResize([](uint32_t width, uint32_t height) {
    Console::Info("Window Resized: {0} {1}", Console::Arguments() << width << height);
  });
  Core::Input::NotifyOnWindowFocus([](bool isFocused){
    Console::Info("Window Focused: {0}", isFocused);
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