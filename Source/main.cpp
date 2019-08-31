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
    transform->Position = glm::vec3(0, 10, 30);
    transform->Rotation = glm::vec4(0, 0, 0, 1);

    const auto comp = camera->AddComponent<Component::Camera>();
    comp->ViewportOffset = glm::vec2(0, 0);
    comp->ViewportSize = glm::vec2(0.5f, 1.0f);
  }

  const auto camera2 = Core::Entity::Create();
  {
    const auto transform = camera2->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 3, 15);
    transform->Rotation = glm::vec4(0, 0, 0, 1);

    const auto comp = camera2->AddComponent<Component::Camera>();
    comp->ViewportOffset = glm::vec2(0.5f, 0);
    comp->ViewportSize = glm::vec2(0.5f, 1.0f);
  }

  const auto light = Core::Entity::Create();
  {
    const auto transform = light->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(3, 5, 3);

    light->AddComponent<Component::Light>();
  }

  const auto dragon = Core::Entity::Create();
  {
    //transform data
    const auto transform = dragon->AddComponent<Component::Transform>();
    transform->Position = glm::vec3(0, 0, 0);
    transform->Rotation = glm::vec4(0, 0, 0, 1);
    transform->Scale = glm::vec3(1, 1, 1);

    //mesh data
    const auto model = Utils::IO::LoadObjFile("Models/Dragon.obj");
    const auto mesh = dragon->AddComponent<Component::Mesh>();
    mesh->SetVertices(model.Vertices);
    mesh->SetIndices(model.Indices);
    mesh->AutoFetchLights(true);
  }

  float yRotation = 0.0f;

  //Main Loop
  while (!Core::Screen::PollEvents())
  {
    dragon->GetComponent<Component::Transform>()->Rotation = glm::vec4(0, yRotation, 0, 1);

    //iterate through all system and execute update functions
    Core::IterateSystemLoop();
    yRotation += 0.00001;
  }

  //remove components
  camera->RemoveComponent<Component::Transform>();
  camera->RemoveComponent<Component::Camera>();
  camera2->RemoveComponent<Component::Transform>();
  camera2->RemoveComponent<Component::Camera>();
  dragon->RemoveComponent<Component::Transform>();
  dragon->RemoveComponent<Component::Mesh>();
  light->RemoveComponent<Component::Transform>();
  light->RemoveComponent<Component::Light>();

  //destroy entities
  Core::Entity::Destroy(camera);
  Core::Entity::Destroy(camera2);
  Core::Entity::Destroy(dragon);
  Core::Entity::Destroy(light);

  //destroy systems
  Core::DestroySystem<Systems::Rendering>();

  return 0;
}