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
    transform->Rotation = glm::vec4(0, 0, 0, 1);

    const auto comp = camera->AddComponent<Component::Camera>();
    comp->ViewportOffset = glm::vec2(0, 0);
    comp->ViewportSize = glm::vec2(0.5f, 1.0f);
  }

  const auto camera2 = Core::Entity::Create();
  {
    const auto transform = camera2->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 0, -3);
    transform->Rotation = glm::vec4(0, 0, 0, 1);

    const auto comp = camera2->AddComponent<Component::Camera>();
    comp->ViewportOffset = glm::vec2(0.5f, 0);
    comp->ViewportSize = glm::vec2(0.5f, 1.0f);
  }

  const auto light = Core::Entity::Create();
  {
    const auto transform = light->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(-1, 5, -1);

    light->AddComponent<Component::Light>();
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
    mesh->SetVertices(model.Vertices);
    mesh->SetIndices(model.Indices);
  }

  const auto monkey = Core::Entity::Create();
  {
    //transform data
    const auto transform = monkey->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 0, 0);
    transform->Rotation = glm::vec4(0, 0, 0, 1);
    transform->Scale = glm::vec3(1, 1, 1);

    //mesh data
    const auto model = Utils::IO::LoadObjFile("Models/Monkey.obj");
    const auto mesh = monkey->AddComponent<Component::Mesh>();
    mesh->SetVertices(model.Vertices);
    mesh->SetIndices(model.Indices);
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

    monkey->GetComponent<Component::Transform>()->Rotation = glm::vec4(0, yRotation, 0, 1);

    //iterate through all system and execute update functions
    Core::IterateSystemLoop();
    yRotation += 0.00001;
  }

  //remove components
  camera->RemoveComponent<Component::Transform>();
  camera->RemoveComponent<Component::Camera>();
  camera2->RemoveComponent<Component::Transform>();
  camera2->RemoveComponent<Component::Camera>();
  cube->RemoveComponent<Component::Transform>();
  cube->RemoveComponent<Component::Mesh>();
  monkey->RemoveComponent<Component::Transform>();
  monkey->RemoveComponent<Component::Mesh>();
  light->RemoveComponent<Component::Transform>();
  light->RemoveComponent<Component::Light>();

  //destroy entities
  Core::Entity::Destroy(cube);
  Core::Entity::Destroy(camera);
  Core::Entity::Destroy(camera2);
  Core::Entity::Destroy(monkey);
  Core::Entity::Destroy(light);

  //destroy systems
  Core::DestroySystem<Systems::Rendering>();

  return 0;
}