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
}; // namespace Tortuga