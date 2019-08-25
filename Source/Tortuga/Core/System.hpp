#ifndef _SYSTEM
#define _SYSTEM

#include <cstring>

#include "unordered_map"
#include "typeindex"
#include <thread>
#include <future>

#include "./Entity.hpp"

namespace Tortuga
{
namespace Core
{
class System
{
public:
  virtual void Update();
  virtual void OnCreate();
  virtual void OnDestroy();
  virtual ~System() = default;
};
std::unordered_map<std::type_index, System *> GetSystemManager();
void SetSystemManager(std::unordered_map<std::type_index, System *> data);
void IterateSystemLoop();
template <typename T>
T *CreateSystem()
{
  auto systemManager = GetSystemManager();
  auto type = std::type_index(typeid(T));
  if (systemManager[type] != nullptr)
    return dynamic_cast<T *>(systemManager[type]);

  systemManager[type] = dynamic_cast<System *>(new T());
  systemManager[type]->OnCreate();
  SetSystemManager(systemManager);
  return dynamic_cast<T *>(systemManager[type]);
}
template <typename T>
void DestroySystem()
{
  auto systemManager = GetSystemManager();
  auto type = std::type_index(typeid(T));
  if (systemManager[type] == nullptr)
    return;

  systemManager[type]->OnDestroy();
  delete dynamic_cast<T *>(systemManager[type]);
  systemManager.erase(type);
  SetSystemManager(systemManager);
}
template <typename T>
T *GetSystem()
{
  auto systemManager = GetSystemManager();
  auto type = std::type_index(typeid(T));
  if (systemManager[type] != nullptr)
    return dynamic_cast<T *>(systemManager[type]);

  return nullptr;
}
} // namespace Core
} // namespace Tortuga

#endif