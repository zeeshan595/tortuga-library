#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Core::Screen::SetWindowTitle("Hello World");
  Core::Screen::ResizeWindow(1920, 1080);

  //Start rendering system
  Core::CreateSystem<Systems::Rendering>();

  //create entity
  auto entity = Core::Entity::Create();
  //attach transform and mesh component
  entity->AddComponent<Component::Transform>();
  entity->AddComponent<Component::Mesh>();

  //Main Loop
  bool shouldClose = false;
  while (!shouldClose)
  {
    //Window events
    auto event = Core::Screen::PollEvents();
    //Quit on close pressed
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      shouldClose = true;
  }

  //remove transform and mesh component
  entity->RemoveComponent<Component::Mesh>();
  entity->RemoveComponent<Component::Transform>();

  //destroy entity
  Core::Entity::Destroy(entity);

  //destroy rendering system
  Core::DestroySystem<Systems::Rendering>();
  return 0;
}