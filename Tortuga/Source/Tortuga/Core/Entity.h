#ifndef _CORE_ENTITY
#define _CORE_ENTITY

#include <any>
#include <typeindex>
#include <unordered_map>

namespace Tortuga {
namespace Core {
struct Entity {
private:
  std::unordered_map<std::type_index, std::any> Components;

public:
  template <typename T> void AddComponent(T data = T()) {
    Components[std::type_index(typeid(T))] = data;
  }

  template <typename T> void RemoveComponent() {
    Components.erase(std::type_index(typeid(T)));
  }

  template <typename T> T GetComponent() {
    return Components[std::type_index(typeid(T))];
  }
};
} // namespace Core
} // namespace Tortuga

#endif