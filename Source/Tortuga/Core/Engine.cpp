#include "./Engine.hpp"

#include <unordered_map>

namespace Tortuga
{
namespace Core
{
namespace Engine
{
struct Engine
{
  std::unordered_map<std::type_index, ECS::System *> Systems;
  std::vector<ECS::Entity *> entities;
  std::unordered_map<std::type_index, std::vector<ECS::Component *>> Components;

  ~Engine()
  {
    for (auto i = Systems.begin(); i != Systems.end(); ++i)
      delete i->second;

    for (const auto entity : entities)
      delete entity;
  }
};
auto __global = Engine();

//systems
void AddSystem(std::type_index type, ECS::System *data)
{
  if (__global.Systems.find(type) != __global.Systems.end())
    return;

  __global.Systems.insert(std::pair(type, data));
}
void RemoveSystem(std::type_index type)
{
  if (__global.Systems.find(type) == __global.Systems.end())
    return;
  delete __global.Systems[type];
  __global.Systems.erase(type);
}
ECS::System *GetSystem(std::type_index type)
{
  if (__global.Systems.find(type) == __global.Systems.end())
    return nullptr;
  return __global.Systems[type];
}
void IterateSystems()
{
  for (auto i = __global.Systems.begin(); i != __global.Systems.end(); ++i)
    i->second->Update();
}
//entity
ECS::Entity *CreateEntity()
{
  auto data = new ECS::Entity();
  __global.entities.push_back(data);
  return data;
}
void DestroyEntity(ECS::Entity *entity)
{
  for (auto i = __global.entities.begin(); i != __global.entities.end(); ++i)
  {
    if ((*i) == entity)
    {
      for (auto j = entity->Components.begin(); j != entity->Components.end(); ++j)
      {
        auto comp = __global.Components[(*j).first];
        for (auto k = comp.begin(); k != comp.end(); ++k)
        {
          if ((*k)->object == entity)
          {
            comp.erase(k);
            __global.Components[(*j).first] = comp;
            break;
          }
        }
      }
      __global.entities.erase(i);
      delete entity;
      break;
    }
  }
}
void AddComponent(ECS::Entity *entity, std::type_index type, ECS::Component *data)
{
  data->object = entity;
  entity->Components.insert(std::pair(type, data));
  if (__global.Components.find(type) == __global.Components.end())
  {
    std::vector<ECS::Component *> dataList = {data};
    __global.Components.insert(std::pair(type, dataList));
  }
  else
  {
    __global.Components[type].push_back(data);
  }
}
void RemoveComponent(ECS::Entity *entity, std::type_index type)
{
  if (entity->Components.find(type) == entity->Components.end())
    return;

  delete entity->Components[type];
  entity->Components.erase(type);
  if (__global.Components.find(type) == __global.Components.end())
    return;

  auto comp = __global.Components[type];
  for (auto k = comp.begin(); k != comp.end(); ++k)
  {
    if ((*k)->object == entity)
    {
      comp.erase(k);
      __global.Components[type] = comp;
      break;
    }
  }
}
ECS::Component *GetComponent(ECS::Entity *entity, std::type_index type)
{
  return entity->Components[type];
}
std::vector<ECS::Component *> GetComponents(std::type_index type)
{
  return __global.Components[type];
}
} // namespace Engine
} // namespace Core
} // namespace Tortuga