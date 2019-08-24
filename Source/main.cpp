#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Core::Screen::SetWindowTitle("Hello World");
  Core::Screen::ResizeWindow(800, 600);

  //Start rendering system
  Core::CreateSystem<Systems::Rendering>();

  //create entity
  auto entity = Core::Entity::Create();

  auto cube = Utils::IO::LoadObjFile("Models/Cube.obj");

  Component::Mesh meshData = {};
  meshData.SetVertices(cube.Vertices, true);
  meshData.SetIndices(cube.Indices);
  meshData.SetDynamic();

  Component::Transform transformData = {};
  transformData.Position = glm::vec3(0, 0, 10);
  transformData.Rotation = glm::vec4(0, 0, 0, 1);
  transformData.Scale = glm::vec3(1, 1, 1);

  //attach transform and mesh component
  entity->AddComponent<Component::Transform>(&transformData);
  entity->AddComponent<Component::Mesh>(&meshData);

  float yPosition = 0.0f;
  bool isDone = false;

  //Main Loop
  bool shouldClose = false;
  while (!shouldClose)
  {
    //Window events
    auto event = Core::Screen::PollEvents();
    //Quit on close pressed
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      shouldClose = true;

    entity->GetComponent<Component::Transform>()->Rotation = glm::vec4(0, yPosition, 0, 1);

    if (event.type == SDL_KEYDOWN)
    {
      if (event.key.keysym.sym == SDLK_SPACE && !isDone)
      {
        Core::Screen::ResizeWindow(1920, 1080);
        isDone = true;
      }
    }

    //iterate through all system and execute update functions
    Core::IterateSystemLoop();
    yPosition += 0.00001;
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