#include "./System.hpp"

namespace Tortuga
{
namespace Core
{
void System::Update() {}
std::unordered_map<std::type_index, System *> SystemManager = std::unordered_map<std::type_index, System *>();
std::unordered_map<std::type_index, System *> GetSystemManager()
{
  return SystemManager;
}
void SetSystemManager(std::unordered_map<std::type_index, System *> data)
{
  SystemManager = data;
}

void IterateSystemLoop()
{
  for (auto system : GetSystemManager())
  {
    if (system.second != nullptr)
      system.second->Update();
  }
}
} // namespace Core
} // namespace Tortuga