#include "./Engine.hpp"

#include <vector>
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

  ~Engine()
  {
    for (auto i = Systems.begin(); i != Systems.end(); ++i)
      delete i->second;
  }
};
auto __global = Engine();

void AddSystem(std::type_index type, ECS::System *data)
{
  if (__global.Systems.find(type) != __global.Systems.end())
    return;

  __global.Systems[type] = data;
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
} // namespace Engine
} // namespace Core
} // namespace Tortuga