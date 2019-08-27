#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Core::Screen::SetWindowTitle("Hello World");
  Core::Screen::ResizeWindow(1920, 1080);

  //Start rendering system
  Core::CreateSystem<Systems::Rendering>();

  //create cube entity
  auto cube = Core::Entity::Create();
  {
    //transform data
    auto transform = cube->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 0, 10);
    transform->Rotation = glm::vec4(0, 0, 0, 1);
    transform->Scale = glm::vec3(1, 1, 1);

    //mesh data
    auto model = Utils::IO::LoadObjFile("Models/Cube.obj");
    auto mesh = cube->AddComponent<Component::Mesh>();
  }

  //create light entity
  auto light = Core::Entity::Create();
  {
    auto transform = light->AddComponent<Component::Transform>();
    transform->Position = glm::vec4(1, 5, 10, 1);
    transform->Rotation = glm::vec4(0, 0, 0, 1);
    transform->Scale = glm::vec4(1, 1, 1, 1);

    auto lightComponent = light->AddComponent<Component::Light>();
    lightComponent->SetStatic(true);
  }

  float yRotation = 0.0f;

  //Main Loop
  bool shouldClose = false;
  while (!shouldClose)
  {
    //Window events
    auto event = Core::Screen::PollEvents();
    //Quit on close pressed
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      shouldClose = true;

    //cube->GetComponent<Component::Transform>()->Rotation = glm::vec4(0, yRotation, 0, 1);

    //iterate through all system and execute update functions
    Core::IterateSystemLoop();
    yRotation += 0.00001;
  }

  //remove components
  cube->RemoveComponent<Component::Transform>();
  cube->RemoveComponent<Component::Mesh>();
  light->RemoveComponent<Component::Transform>();
  light->RemoveComponent<Component::Light>();

  //destroy systems
  Core::DestroySystem<Systems::Rendering>();

  //destroy cube
  Core::Entity::Destroy(cube);
  Core::Entity::Destroy(light);

  return 0;
}