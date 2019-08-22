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

  Component::Mesh meshData = {};
  meshData.SetVertices(
      {{{0, 1, 0, 1},
        {0, 0, -1, 1}},
       {{-1, 0, 0, 1},
        {0, 0, -1, 1}},
       {{1, 0, 0, 1},
        {0, 0, -1, 1}}},
      true);
  meshData.SetIndices({0, 1, 2});
  meshData.ApplyTransformation({0, 0, 3}, {0, 0, 0, 1}, {1, 1, 1});
  meshData.SetStatic();

  //attach transform and mesh component
  entity->AddComponent<Component::Transform>();
  entity->AddComponent<Component::Mesh>(&meshData);

  //Main Loop
  bool shouldClose = false;
  while (!shouldClose)
  {
    //Window events
    auto event = Core::Screen::PollEvents();
    //Quit on close pressed
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      shouldClose = true;

    //iterate through all system and execute update functions
    Core::IterateSystemLoop();
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