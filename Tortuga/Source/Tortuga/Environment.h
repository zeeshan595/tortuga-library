#ifndef _ENVIRONMENT
#define _ENVIRONMENT

#include "Core.h"
#include "Console.h"
#include "Transformation.h"

namespace Tortuga
{
struct EntityData
{
};
struct Entity
{
  std::string Name;
  Transformation Transform;
  std::vector<EntityData> Data;
};
struct Environment
{
  std::string Name;
  std::vector<Entity *> Entities;
};
Environment CreateEnvironment(std::string name);
Entity *CreateEntity(
    Environment &environment,
    const char *name = "New Entity",
    glm::vec3 position = glm::vec3(0, 0, 0),
    glm::vec4 rotation = glm::vec4(0, 0, 0, 0),
    glm::vec3 scale = glm::vec3(1, 1, 1));

void DestroyEntity(Environment &e, Entity *data);
void DestroyEnvironment(Environment data);

template <typename T>
void AddEntityData(Entity *e)
{
  auto temp = dynamic_cast<EntityData *>(new T());
  if (temp == nullptr)
  {
    Console::Error("Failed to add data structure to entity, All data structures must inherit from `EntityData`");
    return;
  }

  e->Data.push_back(*temp);
  delete temp;
}
template <typename T>
void RemoveEntityData(Entity *e)
{
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    if (typeid(T) == typeid(e->Data[i]))
    {
      e->Data.erase(e->Data.begin() + i);
      break;
    }
  }
}
template <typename T>
void UpdateEntityData(Entity *e, T data)
{
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    if (typeid(T) == typeid(e->Data[i]))
    {
      auto temp = dynamic_cast<EntityData *>(&data);
      e->Data[i] = *temp;
      delete temp;
      break;
    }
  }
}
}; // namespace Tortuga

#endif