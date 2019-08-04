#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  auto data = new Component::Mesh();
  data->BufferData.VerticesSize = 3;
  auto entity = Core::Entity::Create();

  entity->AddComponent<Component::Mesh>(data);
  auto temp = entity->GetComponent<Component::Mesh>();
  Core::CreateSystem<Systems::Rendering>();



  entity->RemoveComponent<Component::Mesh>();
  Core::Entity::Destroy(entity);
  Core::DestroySystem<Systems::Rendering>();

  return 0;
}