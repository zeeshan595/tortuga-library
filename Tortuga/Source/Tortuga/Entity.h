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
  std::vector<EntityDataStructure> DataStructures;
};
template <typename T>
struct EntityExtractedData
{
  static_assert(std::is_base_of<EntityDataStructure, T>::value, "T must inherit from Key");

  std::string Name;
  Transformation Transform;
  T DataStructure;
};

template <typename T>
void AddEntityDataStructure(Entity *entity)
{
  EntityDataStructure data = (EntityDataStructure)T();
  entity->DataStructures.push_back(data);
}

template <typename T>
void RemoveEntityDataStructure(Entity *entity)
{
  for (uint32_t i = 0; i < entity->DataStructures.size(); i++)
  {
    auto d = entity->DataStructures[i];
  }
}
}; // namespace Tortuga

#endif
