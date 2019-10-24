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
public:
  bool DestroyOnStartOfLoop = false;
  Entity *Root = nullptr;

  virtual void OnCreate() {}  //is called when attached to an entity
  virtual void OnDestroy() {} //is called when removed from an entity
  virtual ~Component() {}     //allow proper inheritance
};
struct Entity
{
  std::unordered_map<std::type_index, Component *> Components;

  ~Entity()
  {
    for (auto i = Components.begin(); i != Components.end(); ++i)
    {
      if (i->second->DestroyOnStartOfLoop == false)
      {
        i->second->OnDestroy();
        delete i->second;
      }
    }
  }
};
} // namespace ECS
} // namespace Core
} // namespace Tortuga

#endif