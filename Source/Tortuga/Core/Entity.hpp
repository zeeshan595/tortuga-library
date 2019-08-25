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

    return temp;
  }
  template <typename T>
  void RemoveComponent()
  {
    auto type = std::type_index(typeid(T));
    if (this->Components.find(type) == this->Components.end())
      return;

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
  ~Environment();
};
Entity *Create();
void Destroy(Entity *data);
std::vector<Entity *> GetAllEntities();
} // namespace Entity
} // namespace Core
} // namespace Tortuga

#endif