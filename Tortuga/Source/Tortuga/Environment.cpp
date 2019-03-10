#include "Environment.h"

namespace Tortuga
{
Environment *CreateEnvironment()
{
  auto data = new Environment();
  return data;
}
void DestroyEnvironment(Environment *env)
{
  for (uint32_t i = 0; i < env->Entities.size(); i++)
    delete env->Entities[i];

  delete env;
}

int32_t FindEntity(Environment *env, Entity *entity)
{
  for (uint32_t i = 0; i < env->Entities.size(); i++)
  {
    if (env->Entities[i] == entity)
    {
      return i;
    }
  }
  return -1;
}
Entity *CreateEntity(Environment *env)
{
  auto data = new Entity();
  env->Entities.push_back(data);
  return data;
}
void DestroyEntity(Environment *env, Entity *entity)
{
  auto index = FindEntity(env, entity);
  if (index == -1)
    return;

  env->Entities.erase(env->Entities.begin() + index);
  delete entity;
}

std::vector<EntityExtractedData<EntityDataStructure>> ExtractEntitiesDataStructures(Environment *env, std::type_info &typeInfo)
{
  std::vector<EntityExtractedData<EntityDataStructure>> data;
  for (uint32_t i = 0; i < env->Entities.size(); i++)
  {
    for (uint32_t j = 0; j < env->Entities[i]->DataStructures.size(); i++)
    {
      if (typeid(env->Entities[i]->DataStructures[j]) == typeInfo)
      {
        auto extraction = EntityExtractedData<EntityDataStructure>();
        {
          extraction.Name = env->Entities[i]->Name;
          extraction.Transform = env->Entities[i]->Transform;
          extraction.DataStructure = env->Entities[i]->DataStructures[j];
        }
        data.push_back(extraction);
        break;
      }
    }
  }
  return data;
}
}; // namespace Tortuga