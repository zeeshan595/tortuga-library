#ifndef _ENTITY
#define _ENTITY

#include "Core.h"
#include "Transformation.h"

namespace Tortuga
{
struct EntityDataStructure
{
protected:
  std::string TypeInfo;

public:
  bool IsEnabled = true;

  std::string GetTypeInfo() { return TypeInfo; }
  EntityDataStructure() : TypeInfo(std::string(typeid(EntityDataStructure).name())) {}
  EntityDataStructure(std::string typeInfo)
  {
    this->TypeInfo = typeInfo;
  }
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

  Entity *Reference;
  std::string Name;
  Transformation Transform;
  T Data;
};

template <typename T>
void AddEntityDataStructure(Entity *entity)
{
  entity->DataStructures.push_back(T());
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
