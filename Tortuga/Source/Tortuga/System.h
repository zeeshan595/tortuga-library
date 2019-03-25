#ifndef _SYSTEM
#define _SYSTEM

#include "./Core.h"
#include "./Console.h"
#include "./Environment.h"

namespace Tortuga
{
class System;
struct SystemController
{
    std::unordered_map<std::type_index, System *> Systems;
    std::vector<System *> SystemsVector;
};
class System
{
  private:
    std::unordered_map<std::type_index, std::vector<ComponentData<std::any>>> _componentsData;
    std::vector<std::type_index> _registeredTypes;

  protected:
    template <typename T>
    std::vector<ComponentData<T>> GetData()
    {
        if (_componentsData.find(std::type_index(typeid(T))) == _componentsData.end())
            return {};

        auto ext = _componentsData[std::type_index(typeid(T))];
        std::vector<ComponentData<T>> data(ext.size());
        for (uint32_t i = 0; i < data.size(); i++)
        {
            data[i] = ComponentData<T>();
            data[i].Reference = ext[i].Reference;
            data[i].Data = std::any_cast<T>(ext[i].Data);
        }

        return data;
    }
    template <typename T>
    void SetData(std::vector<ComponentData<T>> data)
    {
        auto typeInfo = std::type_index(typeid(T));
        if (_componentsData.find(typeInfo) == _componentsData.end())
        {
            Console::Warning("Trying to set a component that is not registered is not allowed");
            return;
        }
        if (_componentsData[typeInfo].size() != data.size())
        {
            Console::Warning("The data provided does not match the amount of entities that have the data");
            return;
        }
        _componentsData[typeInfo] = data;
    }
    template <typename T>
    void RegisterComponent()
    {
        auto t = std::type_index(typeid(T));
        for (uint32_t i = 0; i < _registeredTypes.size(); i++)
        {
            if (_registeredTypes[i] == t)
            {
                Console::Warning("This component is already registered with this system");
                return;
            }
        }
        _registeredTypes.push_back(t);
    }
    template <typename T>
    void UnRegisterComponent()
    {
        auto t = std::type_index(typeid(T));
        for (auto i = _registeredTypes.begin(); i < _registeredTypes.end(); i++)
        {
            if (t == *i)
            {
                _registeredTypes.erase(i);
                break;
            }
        }
    }

  public:
    std::vector<std::type_index> GetRegisteredTypes()
    {
        return _registeredTypes;
    }

    void UpdateComponentData(std::unordered_map<std::type_index, std::vector<ComponentData<std::any>>> componentsData)
    {
        _componentsData = componentsData;
    }
    std::unordered_map<std::type_index, std::vector<ComponentData<std::any>>> FetchComponentData()
    {
        return _componentsData;
    }

    virtual void OnStart() {}
    virtual void OnUpdate() {}
    virtual void OnEnd() {}

    virtual ~System() {}
};

SystemController *CreateSystemController();
void DestroySystemController(SystemController *controller);
void ProcessSystems(SystemController *controller, Environment *env);

template <typename T>
T *AddSystem(SystemController *controller)
{
    if (controller->Systems.find(std::type_index(typeid(T))) != controller->Systems.end())
    {
        Console::Warning("Trying to add a system that is already added in this system controller");
        return nullptr;
    }
    auto data = new T();
    controller->Systems[std::type_index(typeid(T))] = data;
    controller->SystemsVector.push_back(data);
    controller->Systems[std::type_index(typeid(T))]->OnStart();
    return data;
}

template <typename T>
void RemoveSystem(SystemController *controller)
{
    if (controller->Systems.find(std::type_index(typeid(T))) == controller->Systems.end())
    {
        Console::Warning("Trying to remote a system that is does not already in this system controller");
        return;
    }
    auto ref = controller->Systems[std::type_index(typeid(T))];
    ref->OnEnd();
    controller->Systems.erase(std::type_index(typeid(T)));
    for (auto i = controller->SystemsVector.begin(); i < controller->SystemsVector.end(); i++)
    {
        if (ref == (*i))
        {
            controller->SystemsVector.erase(i);
            break;
        }
    }
    delete ref;
}
}; // namespace Tortuga

#endif