#ifndef _ENTITY
#define _ENTITY

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <any>
#include "./GUID.hpp"
#include "../Console.hpp"

namespace Tortuga
{
namespace Core
{
namespace Entity
{
struct Environment;
struct Entity
{
private:
  std::string GUID;
  std::unordered_map<std::type_index, void *> Components;
  std::unordered_map<std::type_index, void *> ComponentsToCleanUp;
  Environment *GetEnvironment();

public:
  template <typename T>
  T *AddComponent(T *data = nullptr)
  {
    auto type = std::type_index(typeid(T));
    if (this->Components.find(type) != this->Components.end())
      return static_cast<T *>(this->Components[type]);

    T *temp = nullptr;
    if (data != nullptr)
      temp = data;
    else
      temp = new T();
    this->Components[type] = temp;

    auto environment = GetEnvironment();
    if (environment->EntitiesWithComponent.find(type) != environment->EntitiesWithComponent.end())
      environment->EntitiesWithComponent[type].push_back(this);
    else
      environment->EntitiesWithComponent[type] = {this};

    return temp;
  }
  template <typename T>
  void RemoveComponent()
  {
    auto type = std::type_index(typeid(T));
    if (this->Components.find(type) == this->Components.end())
      return;

    auto environment = GetEnvironment();
    auto entityWithComponentIndex = std::find(environment->EntitiesWithComponent[type].begin(), environment->EntitiesWithComponent[type].end(), this);
    GetEnvironment()->EntitiesWithComponent[type].erase(entityWithComponentIndex);
    delete static_cast<T *>(this->Components[type]);
    this->Components.erase(type);
  }
  template <typename T>
  T *GetComponent()
  {
    auto type = std::type_index(typeid(T));
    if (this->Components.find(type) == this->Components.end())
      return nullptr;
    else
      return static_cast<T *>(this->Components[type]);
  }

  Entity();
  ~Entity();
  std::string GetGUID();
};
struct Environment
{
  std::vector<Entity *> Entities;
  std::unordered_map<std::type_index, std::vector<Entity *>> EntitiesWithComponent;
  ~Environment();
};
Entity *Create();
void Destroy(Entity *data);
std::vector<Entity *> GetAllEntities();
std::vector<Entity *> GetALlEntitiesWithComponent(std::type_index type);
template <typename T>
std::vector<Entity *> GetALlEntitiesWithComponent()
{
  auto type = std::type_index(typeid(T));
  return GetALlEntitiesWithComponent(type);
}
} // namespace Entity
} // namespace Core
} // namespace Tortuga

#endif