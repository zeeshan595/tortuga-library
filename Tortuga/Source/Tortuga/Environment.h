#ifndef _ENVIRONMENT
#define _ENVIRONMENT

#include "Core.h"
#include "Console.h"
#include "Transformation.h"

namespace Tortuga
{
struct DataStructure
{
};
struct Entity
{
  std::string Name;
  Transformation Transform;
  std::vector<DataStructure> Data;
  std::mutex Lock;
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
void AddEntityData(Entity *e, T data = T())
{
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    T *temp = (T *)&e->Data[i];
    if (temp != nullptr)
    {
      Console::Error("Failed to add data structure to entity because this entity already has that data structure!");
      return;
    }
  }
  e->Data.push_back(data);
}
template <typename T>
void RemoveEntityData(Entity *e)
{
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    T *temp = (T *)&e->Data[i];
    if (temp != nullptr)
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
    T *temp = (T *)&e->Data[i];
    if (temp != nullptr)
    {
      e->Data[i] = data;
      break;
    }
  }
}
template <typename T>
T GetEntityData(Entity *e)
{
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    T *temp = (T *)&e->Data[i];
    if (temp != nullptr)
    {
      return e->Data[i];
    }
  }
}

}; // namespace Tortuga

#endif