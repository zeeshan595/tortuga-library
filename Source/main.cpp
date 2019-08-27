#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Core::Screen::SetWindowTitle("Hello World");
  Core::Screen::ResizeWindow(1920, 1080);

  //Start rendering system
  Core::CreateSystem<Systems::Rendering>();

  const auto camera = Core::Entity::Create();
  {
    const auto transform = camera->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 0, -10);
    transform->Rotation = glm::vec4(0, 3.14, 0, 1);

    const auto comp = camera->AddComponent<Component::Camera>();
    comp->FieldOfView = 50.0f;
    comp->aspectRatio = 1920.0f / 1080.0f;
    comp->nearClipPlane = 0.01f;
    comp->farClipPlane = 100.0f;
  }

  //create cube entity
  const auto cube = Core::Entity::Create();
  {
    //transform data
    const auto transform = cube->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 0, 5);
    transform->Rotation = glm::vec4(0, 0, 0, 1);
    transform->Scale = glm::vec3(1, 1, 1);

    //mesh data
    const auto model = Utils::IO::LoadObjFile("Models/Cube.obj");
    const auto mesh = cube->AddComponent<Component::Mesh>();
    mesh->Vertices = {
        {{-0.5f, -0.5f, 0}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f, 0}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0}, {1.0f, 1.0f, 1.0f}}};

    mesh->Indices = {0, 1, 2, 2, 3, 0};
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

    cube->GetComponent<Component::Transform>()->Rotation = glm::vec4(0, yRotation, 0, 1);

    //iterate through all system and execute update functions
    Core::IterateSystemLoop();
    yRotation += 0.00001;
  }

  //remove components
  cube->RemoveComponent<Component::Transform>();
  cube->RemoveComponent<Component::Mesh>();
  camera->RemoveComponent<Component::Transform>();
  camera->RemoveComponent<Component::Camera>();

  //destroy entities
  Core::Entity::Destroy(cube);
  Core::Entity::Destroy(camera);

  //destroy systems
  Core::DestroySystem<Systems::Rendering>();

  return 0;
}