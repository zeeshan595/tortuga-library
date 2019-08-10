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
  virtual void Created() {}
  virtual void Update() {}
  virtual void Destroyed() {}
};
std::unordered_map<std::type_index, System *> SystemManager;
template <typename T>
T *CreateSystem()
{
  auto type = std::type_index(typeid(T));
  if (SystemManager[type] != nullptr)
    return static_cast<T *>(SystemManager[type]);

  SystemManager[type] = static_cast<System *>(new T());
  SystemManager[type]->Created();
  return static_cast<T *>(SystemManager[type]);
}
template <typename T>
void DestroySystem()
{
  auto type = std::type_index(typeid(T));
  if (SystemManager[type] == nullptr)
    return;

  SystemManager[type]->Destroyed();
  delete SystemManager[type];
  SystemManager.erase(type);
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