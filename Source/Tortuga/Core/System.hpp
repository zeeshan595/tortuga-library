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
  virtual void Update() {}

  virtual ~System() = default;
};
std::unordered_map<std::type_index, System *> SystemManager;
template <typename T>
T *CreateSystem()
{
  auto type = std::type_index(typeid(T));
  if (SystemManager[type] != nullptr)
    return dynamic_cast<T *>(SystemManager[type]);

  SystemManager[type] = dynamic_cast<System *>(new T());
  return dynamic_cast<T *>(SystemManager[type]);
}
template <typename T>
void DestroySystem()
{
  auto type = std::type_index(typeid(T));
  if (SystemManager[type] == nullptr)
    return;

  delete dynamic_cast<T *>(SystemManager[type]);
  SystemManager.erase(type);
}
template <typename T>
T *GetSystem()
{
  auto type = std::type_index(typeid(T));
  if (SystemManager[type] != nullptr)
    return dynamic_cast<T *>(SystemManager[type]);

  return nullptr;
}
void IterateSystemLoop()
{
  for (auto system : SystemManager)
  {
    if (system.second != nullptr)
      system.second->Update();
  }
}
} // namespace Core
} // namespace Tortuga

#endif