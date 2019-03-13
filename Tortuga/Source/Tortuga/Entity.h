#ifndef _ENTITY
#define _ENTITY

#include "Core.h"
#include "Transformation.h"

namespace Tortuga
{
struct EntityDataStructure
{
};
struct Entity
{
  std::string Name;
  Transformation Transform;
  std::unordered_map<std::type_index, EntityDataStructure> DataStructures;
};
template <typename T>
struct EntityExtractedData
{
  static_assert(std::is_base_of<EntityDataStructure, T>::value, "T must inherit from Key");

private:
  Entity *_entityRef;

public:
  T Data;

  EntityExtractedData(Entity *entityRef)
  {
    _entityRef = entityRef;
  }
  Entity *GetEntityReference()
  {
    return _entityRef;
  }
};

template <typename T>
void AddEntityDataStructure(Entity *entity)
{
  EntityDataStructure data = T();
  entity->DataStructures[std::type_index(typeid(T))] = data;
}

template <typename T>
void RemoveEntityDataStructure(Entity *entity)
{
  entity->DataStructures.erase(std::type_index(typeid(T)));
}
}; // namespace Tortuga

#endif
