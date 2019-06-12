#ifndef _CORE_SYSTEM
#define _CORE_SYSTEM

#include <any>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace Tortuga {
namespace Core {
struct System {
private:
  std::vector<std::type_index> RegisteredComponents;
  std::unordered_map<std::type_index, std::vector<std::any>> Components;

  int32_t IsComponentRegistered(std::type_index &typeIndex) {
    for (uint32_t i = 0; i < RegisteredComponents.size(); i++) {
      if (RegisteredComponents[i] == typeIndex) {
        return i;
      }
    }

    return -1;
  }

protected:
  template <typename T> void RegisterComponent(T data = T()) {
    auto typeIndex = std::type_index(typeid(T));
    if (IsComponentRegistered(typeIndex) != -1)
      return;
    RegisteredComponents.push_back(typeIndex);
  }
  template <typename T> void UnRegisterComponent() {
    auto typeIndex = std::type_index(typeid(T));
    int32_t index = IsComponentRegistered(typeIndex);
    if (index == -1)
      return;
    RegisteredComponents.erase(RegisteredComponents.begin() + index);
  }
  template <typename T> std::vector<T> GetComponentData() {
    return Components[std::type_index(typeid(T))];
  }

public:
  System() {
    RegisteredComponents = {};
    Components = {};
  }
  virtual ~System() = default;
  virtual void Start() {}
  virtual void Update() {}
  virtual void End() {}
};
std::unordered_map<std::type_index, System*> Systems;

template <typename T> void CreateSystem() {
  auto typeIndex = std::type_index(typeid(T));
  Systems[typeIndex] = new T();
}
template <typename T> void DestroySystem() {
  auto typeIndex = std::type_index(typeid(T));
  delete &Systems[typeIndex];
  Systems.erase(typeIndex);
}
} // namespace Core
} // namespace Tortuga

#endif