#include "./Engine.hpp"

#include <unordered_map>
#include "./Console.hpp"

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
  std::vector<ECS::Component *> ComponentsToDestroy;

  Engine() {}
  ~Engine()
  {
    for (auto i = Systems.begin(); i != Systems.end(); ++i)
      delete i->second;

    for (const auto entity : entities)
      delete entity;
  }
};
Engine *engine = nullptr;
void Create()
{
  engine = new Engine();
}
void Destroy()
{
  delete engine;
}

//systems
void AddSystem(std::type_index type, ECS::System *data)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return;
  }

  if (engine->Systems.find(type) != engine->Systems.end())
    return;

  engine->Systems.insert(std::pair(type, data));
}
void RemoveSystem(std::type_index type)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return;
  }

  if (engine->Systems.find(type) == engine->Systems.end())
    return;
  delete engine->Systems[type];
  engine->Systems.erase(type);
}
ECS::System *GetSystem(std::type_index type)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return nullptr;
  }

  if (engine->Systems.find(type) == engine->Systems.end())
    return nullptr;
  return engine->Systems[type];
}
void IterateSystems()
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return;
  }

  for (auto i = engine->ComponentsToDestroy.begin(); i != engine->ComponentsToDestroy.end(); ++i)
  {
    (*i)->OnDestroy();
    delete (*i);
  }
  engine->ComponentsToDestroy.clear();

  for (auto i = engine->Systems.begin(); i != engine->Systems.end(); ++i)
    i->second->Update();
}
//entity
ECS::Entity *CreateEntity()
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return nullptr;
  }

  auto data = new ECS::Entity();
  engine->entities.push_back(data);
  return data;
}
void DestroyEntity(ECS::Entity *entity)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return;
  }

  for (auto i = engine->entities.begin(); i != engine->entities.end(); ++i)
  {
    if ((*i) == entity)
    {
      for (auto j = entity->Components.begin(); j != entity->Components.end(); ++j)
      {
        auto comp = engine->Components[(*j).first];
        for (auto k = comp.begin(); k != comp.end(); ++k)
        {
          if ((*k)->Root == entity)
          {
            if ((*k)->DestroyOnStartOfLoop)
              engine->ComponentsToDestroy.push_back(*k);
            comp.erase(k);
            engine->Components[(*j).first] = comp;
            break;
          }
        }
      }
      engine->entities.erase(i);
      delete entity;
      break;
    }
  }
}
void AddComponent(ECS::Entity *entity, std::type_index type, ECS::Component *data)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return;
  }
  if (entity->Components.find(type) != entity->Components.end())
    return;

  data->Root = entity;
  entity->Components.insert(std::pair(type, data));
  if (engine->Components.find(type) == engine->Components.end())
  {
    std::vector<ECS::Component *> dataList = {data};
    engine->Components.insert(std::pair(type, dataList));
  }
  else
  {
    engine->Components[type].push_back(data);
  }
  entity->Components[type]->OnCreate();
}
void RemoveComponent(ECS::Entity *entity, std::type_index type)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return;
  }

  if (entity->Components.find(type) == entity->Components.end())
    return;

  entity->Components[type]->OnDestroy();
  delete entity->Components[type];
  entity->Components.erase(type);
  if (engine->Components.find(type) == engine->Components.end())
    return;

  auto comp = engine->Components[type];
  for (auto k = comp.begin(); k != comp.end(); ++k)
  {
    if ((*k)->Root == entity)
    {
      comp.erase(k);
      engine->Components[type] = comp;
      break;
    }
  }
}
ECS::Component *GetComponent(ECS::Entity *entity, std::type_index type)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return nullptr;
  }
  if (entity->Components.find(type) == entity->Components.end())
    return nullptr;

  return entity->Components[type];
}
void SetComponent(ECS::Entity *entity, std::type_index type, ECS::Component *data)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return;
  }
  if (entity->Components.find(type) == entity->Components.end())
    return;

  entity->Components[type] = data;
}
std::vector<ECS::Component *> GetComponents(std::type_index type)
{
  if (engine == nullptr)
  {
    Console::Error("You need to create an engine first");
    return {};
  }

  return engine->Components[type];
}
} // namespace Engine
} // namespace Core
} // namespace Tortuga