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

std::vector<EntityExtractedData<EntityDataStructure>> ExtractEntitiesDataStructures(Environment *env, std::type_index typeInfo)
{
  std::vector<EntityExtractedData<EntityDataStructure>> data = {};
  for (uint32_t i = 0; i < env->Entities.size(); i++)
  {
    auto dataFound = env->Entities[i]->DataStructures.find(typeInfo);
    if (dataFound != env->Entities[i]->DataStructures.end())
    {
      auto extraction = EntityExtractedData<EntityDataStructure>(env->Entities[i]);
      extraction.Data = dataFound->second;
      data.push_back(extraction);
      break;
    }
  }
  return data;
}
void PackEntityDataStructures(Environment *env, std::vector<EntityExtractedData<EntityDataStructure>> dataToUpdate)
{
  for (uint32_t i = 0; i < dataToUpdate.size(); i++)
  {
    auto entity = dataToUpdate[i].GetEntityReference();
    auto dataType = std::type_index(typeid(dataToUpdate[i].Data));
    auto itr = entity->DataStructures.find(dataType);
    if (itr == entity->DataStructures.end())
      return;

    entity->DataStructures[dataType] = dataToUpdate[i].Data;
  }
}
}; // namespace Tortuga