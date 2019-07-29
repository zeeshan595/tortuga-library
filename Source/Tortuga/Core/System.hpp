#ifndef _SYSTEM
#define _SYSTEM

#include "./Entity.hpp"

namespace Tortuga
{
namespace Core
{
template <typename T>
struct ComponentData
{
  Entity::Entity *Entity;
  T *Data;
};
class System
{
protected:
  template <typename T>
  std::vector<ComponentData<T>> GetData()
  {
    auto entities = Entity::GetAllEntities();
    std::vector<ComponentData<T>> data(entities.size());
    for (uint32_t i = 0; i < entities.size(); i++)
    {
      data[i].Entity = entities[i];
      data[i].Data = entities[i]->GetComponent<T>();
    }
    return data;
  }

public:
  virtual void Start() {}
  virtual void Update() {}
  virtual void End() {}
};
} // namespace Core
} // namespace Tortuga

#endif