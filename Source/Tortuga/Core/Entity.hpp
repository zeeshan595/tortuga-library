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
struct Entity
{
private:
  std::string GUID;
  std::unordered_map<std::type_index, void *> Components;

public:
  template <typename T>
  void AddComponent(T *data = nullptr)
  {
    auto type = std::type_index(typeid(T));
    if (this->Components[type] != nullptr)
      return;

    if (data != nullptr)
      this->Components[type] = data;
    else
      this->Components[type] = new T();
  }
  template <typename T>
  void RemoveComponent()
  {
    auto type = std::type_index(typeid(T));
    if (this->Components[type] == nullptr)
      return;

    //delete this->Components[type];
    this->Components.erase(type);
  }
  template <typename T>
  T *GetComponent()
  {
    auto type = std::type_index(typeid(T));
    return static_cast<T *>(this->Components[type]);
  }

  Entity()
  {
    this->GUID = Core::GUID::GenerateGUID();
  }
  ~Entity()
  {
    if (this->Components.size() > 0)
      Console::Warning("Please remove all components before destroying the entity");
  }
  std::string GetGUID()
  {
    return this->GUID;
  }
};
struct Environment
{
  std::vector<Entity *> Entities;
  ~Environment();
};
Entity *Create();
void Destroy(Entity *data);
std::vector<Entity *> GetAllEntities();
} // namespace Entity
} // namespace Core
} // namespace Tortuga

#endif