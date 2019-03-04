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
//Please use thread safe functions for this class
struct Entity
{
  std::string Name;
  Transformation Transform;
  std::vector<DataStructure *> Data;
  std::mutex MutexLock;
};
//Please use thread safe functions for this class
struct Environment
{
  std::string Name;
  std::vector<Entity *> Entities;
  std::mutex MutexLock;
};
Environment *CreateEnvironment(std::string name);
Entity *CreateEntity(
    Environment *environment,
    const char *name = "New Entity",
    glm::vec3 position = glm::vec3(0, 0, 0),
    glm::vec4 rotation = glm::vec4(0, 0, 0, 0),
    glm::vec3 scale = glm::vec3(1, 1, 1));

void DestroyEntity(Environment *e, Entity *data);
void DestroyEnvironment(Environment *data);

template <typename T>
std::vector<T> GetEntitiesDataStructures(Environment *e)
{
  std::vector<T> data;
  e->MutexLock.lock();
  for (uint32_t i = 0; i < e->Entities.size(); i++)
  {
    e->Entities[i]->MutexLock.lock();
    for (uint32_t j = 0; j < e->Entities[i]->Data.size(); j++)
    {
      T *temp = static_cast<T *>(e->Entities[i]->Data[i]);
      if (temp != nullptr)
      {
        T d = *temp;
        data.push_back(d);
        break;
      }
    }
    e->Entities[i]->MutexLock.unlock();
  }
  e->MutexLock.unlock();
  return data;
}

template <typename T>
void AddEntityData(Entity *e, T *data = new T())
{
  e->MutexLock.lock();
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    T *temp = static_cast<T *>(e->Data[i]);
    if (temp != nullptr)
    {
      Console::Error("Failed to add data structure to entity because this entity already has that data structure!");
      e->MutexLock.unlock();
      return;
    }
  }
  e->Data.push_back(data);
  e->MutexLock.unlock();
}
template <typename T>
void RemoveEntityData(Entity *e)
{
  e->MutexLock.lock();
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    T *temp = static_cast<T *>(e->Data[i]);
    if (temp != nullptr)
    {
      e->Data.erase(e->Data.begin() + i);
      break;
    }
  }
  e->MutexLock.unlock();
}
template <typename T>
void UpdateEntityData(Entity *e, T data)
{
  e->MutexLock.lock();
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    T *temp = static_cast<T *>(e->Data[i]);
    if (temp != nullptr)
    {
      e->Data[i] = data;
      break;
    }
  }
  e->MutexLock.unlock();
}
template <typename T>
T GetEntityData(Entity *e)
{
  e->MutexLock.lock();
  for (uint32_t i = 0; i < e->Data.size(); i++)
  {
    T *temp = static_cast<T *>(e->Data[i]);
    if (temp != nullptr)
    {
      e->MutexLock.unlock();
      return *temp;
    }
  }
  e->MutexLock.unlock();
  return T();
}

}; // namespace Tortuga

#endif