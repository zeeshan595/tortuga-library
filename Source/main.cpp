#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Core::Screen::SetWindowTitle("Hello World");
  Core::Screen::ResizeWindow(1920, 1080);

  //Start rendering system
  Core::CreateSystem<Systems::Rendering>();

  //create cube entity
  const auto cube = Core::Entity::Create();
  {
    //transform data
    const auto transform = cube->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 0, 10);
    transform->Rotation = glm::vec4(0, 0, 0, 1);
    transform->Scale = glm::vec3(1, 1, 1);

    //mesh data
    const auto model = Utils::IO::LoadObjFile("Models/Cube.obj");
    const auto mesh = cube->AddComponent<Component::Mesh>();
    mesh->Vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
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

  //destroy systems
  Core::DestroySystem<Systems::Rendering>();

  //destroy cube
  Core::Entity::Destroy(cube);

  return 0;
}