#ifndef _ENTITY
#define _ENTITY

#include "./Core.h"
#include "./Console.h"

namespace Tortuga
{
class Entity
{
  private:
    std::unordered_map<std::type_index, std::any> _components;

  public:
  template <typename T>
    T GetComponent()
    {
        if (_components.find(std::type_index(typeid(T))) == _components.end())
        {
            Console::Warning("Trying to get a component that does not exist");
            return T();
        }
        return std::any_cast<T>(_components[std::type_index(typeid(T))]);
    }

    std::any GetComponent(std::type_index typeInfo)
    {
        if (_components.find(typeInfo) == _components.end())
        {
            Console::Warning("Trying to get a component that does not exist");
            return std::any();
        }
        return _components[typeInfo];
    }

    template <typename T>
    void AddComponent(T data = T())
    {
        if (_components.find(std::type_index(typeid(T))) != _components.end())
        {
            Console::Warning("This component is already added on this entity");
            return;
        }
        _components[std::type_index(typeid(T))] = data;
    }

    template <typename T>
    void RemoveComponent()
    {
        _components.erase(std::type_index(typeid(T)));
    }
};
}; // namespace Tortuga

#endif