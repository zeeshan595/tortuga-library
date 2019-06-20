#ifndef _CORE_ENTITY
#define _CORE_ENTITY

#include <any>
#include <typeindex>
#include <unordered_map>

namespace Tortuga {
namespace Core {
struct Entity {
public:
  std::unordered_map<std::type_index, std::any> Components;
  
  template <typename T> void AddComponent(T data = T()) {
    Components[std::type_index(typeid(T))] = data;
  }

  template <typename T> void RemoveComponent() {
    Components.erase(std::type_index(typeid(T)));
  }

  std::any GetComponent(std::type_index typeIndex) {
    return Components[typeIndex];
  }
};
} // namespace Core
} // namespace Tortuga

#endif