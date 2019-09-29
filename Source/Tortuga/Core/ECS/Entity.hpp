#ifndef _ECS_ENTITY
#define _ECS_ENTITY

#include <unordered_map>
#include <typeindex>

namespace Tortuga
{
namespace Core
{
namespace ECS
{
struct Entity;
struct Component
{
  Entity *object;

  virtual ~Component() {} //allow proper inheritance
};
struct Entity
{
  std::unordered_map<std::type_index, Component *> Components;

  ~Entity()
  {
    for (auto i = Components.begin(); i != Components.end(); ++i)
      delete i->second;
  }
};
} // namespace ECS
} // namespace Core
} // namespace Tortuga

#endif