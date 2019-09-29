#ifndef _CORE_ENGINE
#define _CORE_ENGINE

#include <typeindex>
#include "./ECS/System.hpp"

namespace Tortuga
{
namespace Core
{
namespace Engine
{
void AddSystem(std::type_index type, ECS::System *data);
void RemoveSystem(std::type_index type);
ECS::System *GetSystem(std::type_index type);
void IterateSystems();

template <typename T>
void AddSystem()
{
  auto type = std::type_index(typeid(T));
  auto data = new T();
  AddSystem(type, data);
}
template <typename T>
void RemoveSystem()
{
  auto type = std::type_index(typeid(T));
  RemoveSystem(type);
}
template <typename T>
T *GetSystem()
{
  auto type = std::type_index(typeid(T));
  const auto data = GetSystem(type);
  return static_cast<T *>(data);
}
} // namespace Engine
} // namespace Core
} // namespace Tortuga

#endif