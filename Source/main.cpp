#include "./Tortuga.hpp"

using namespace Tortuga;
bool ShouldClose = false;
float rotation = 0.0f;

struct TempSystem : Core::ECS::System
{
  void Update()
  {
    const auto meshes = Core::Engine::GetComponents<Components::Mesh>();
    for (const auto mesh : meshes)
    {
      const auto transform = Core::Engine::GetComponent<Components::Transform>(mesh->Root);
      if (transform != nullptr)
      {
        transform->SetRotation(glm::vec4(0, rotation, 0, 1));
      }
    }
  }
};

int main()
{
  //setup engine
  Core::Engine::Create();

  //setup events (can use these anywhere)
  Core::Input::NotifyOnWindowClose([] {
    ShouldClose = true;
  });
  Core::Input::NotifyOnKeyEvent([](Core::Input::KeyCode key, Core::Input::KeyAction action) {
    if (key == Core::Input::KeyCode::Left)
      rotation += 0.001f;
    else if (key == Core::Input::KeyCode::Right)
      rotation -= 0.001f;
  });

  const auto camera = Core::Engine::CreateEntity();
  {
    Core::Engine::AddComponent<Components::Camera>(camera);
    Components::Transform t;
    t.SetPosition(glm::vec3(0, 2, -7));
    Core::Engine::AddComponent<Components::Transform>(camera, t);
  }

  //create a cube with a mesh component
  const auto cube = Core::Engine::CreateEntity();
  const auto mesh = Utils::IO::LoadObjFile("Assets/Models/Dragon.obj");
  {
    Core::Engine::AddComponent<Components::Mesh>(cube, Components::Mesh(mesh));
    Components::Transform t;
    t.SetScale(glm::vec3(0.5, 0.5, 0.5));
    Core::Engine::AddComponent<Components::Transform>(cube, t);
  }
  //add a rendering system to the engine
  Core::Engine::AddSystem<Systems::Rendering>();
  Core::Engine::AddSystem<TempSystem>();

  //main loop
  while (!ShouldClose)
    Core::Engine::IterateSystems();
  Core::Engine::GetSystem<Systems::Rendering>()->WaitForDevice();

  //auto destroys everything
  Core::Engine::Destroy();
  return EXIT_SUCCESS;
}