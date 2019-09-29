#include "./Tortuga.hpp"

using namespace Tortuga;

struct Mesh : public Core::ECS::Component
{
};

int main()
{
  Core::Engine::AddSystem<Systems::Rendering>();
  Core::Engine::GetSystem<Systems::Rendering>();
  const auto entity = Core::Engine::CreateEntity();
  Core::Engine::AddComponent<Mesh>(entity);
  const auto temp1 = Core::Engine::GetComponent<Mesh>(entity);
  const auto temp2 = Core::Engine::GetComponents<Mesh>();
  Core::Engine::RemoveComponent<Mesh>(entity);
  Core::Engine::DestroyEntity(entity);
  Core::Engine::RemoveSystem<Systems::Rendering>();

  return EXIT_SUCCESS;
  while (true)
  {
    Core::Engine::IterateSystems();
  }
  return EXIT_SUCCESS;
}