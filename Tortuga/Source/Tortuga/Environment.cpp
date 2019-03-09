#include "Environment.h"

namespace Tortuga
{
Environment *CreateEnvironment(std::string name)
{
  auto env = new Environment();
  env->Name = name;
  return env;
}
Entity *CreateEntity(
    Environment *environment,
    const char *name,
    glm::vec3 position,
    glm::vec4 rotation,
    glm::vec3 scale)
{
  environment->MutexLock.lock();
  auto data = new Entity();
  {
    data->Name = name;
    data->Transform.Position = position;
    data->Transform.Rotation = rotation;
    data->Transform.Scale = scale;
  }
  environment->Entities.push_back(data);
  environment->MutexLock.unlock();
  return data;
}

void DestroyEntity(Environment *e, Entity *data)
{
  e->MutexLock.lock();
  data->MutexLock.lock();
  auto i = std::find(e->Entities.begin(), e->Entities.end(), data);
  for (uint32_t i = 0; i < data->Data.size(); i++)
    delete data->Data[i];
  e->Entities.erase(i);
  data->MutexLock.unlock();
  delete data;
  e->MutexLock.unlock();
}
void DestroyEnvironment(Environment *data)
{
  data->MutexLock.lock();
  for (uint32_t i = 0; i < data->Entities.size(); i++)
  {
    data->Entities[i]->MutexLock.lock();
    for (uint32_t j = 0; j < data->Entities[i]->Data.size(); j++)
      delete data->Entities[i]->Data[j];
    data->Entities[i]->MutexLock.unlock();
    delete data->Entities[i];
  }
  data->MutexLock.unlock();
  delete data;
}
}; // namespace Tortuga
