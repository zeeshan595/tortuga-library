#ifndef _SYSTEM_CONTROLLER
#define _SYSTEM_CONTROLLER

#include "Core.h"
#include "Environment.h"
#include "System.h"

namespace Tortuga
{
struct SystemController
{
  std::vector<System<EntityDataStructure> *> AttachedSystems;
  std::vector<std::string> DataTypes;
};

SystemController *CreateSystemController();
void DestroySystemController(SystemController *controller);
void ProcessSystemController(SystemController *controller, Environment *env);
int32_t FindDataType(SystemController *controller, std::string typeInfo);

template <typename T>
int32_t FindSystem(SystemController *controller)
{
  for (uint32_t i = 0; i < controller->AttachedSystems.size(); i++)
  {
    auto data = (System<T>)controller->AttachedSystems[i];
    if (data != nullptr)
      return i;
  }
  return -1;
}

template <typename T>
void AddSystem(SystemController *controller)
{
  auto data = (System<EntityDataStructure> *)(new T());
  if (data == nullptr)
    return;

  data->OnStart();
  auto newTypes = data->GetTypeInfos();
  for (uint32_t i = 0; i < newTypes.size(); i++)
    controller->DataTypes.push_back(newTypes[i]);
    
  controller->AttachedSystems.push_back(data);
}

template <typename T>
void RemoveSystem(SystemController *controller)
{
  auto index = FindSystem<T>(controller);
  if (index == -1)
    return;

  controller->AttachedSystems[index]->OnEnd();
  delete controller->AttachedSystems[index];
  controller->AttachedSystems.erase(controller->AttachedSystems.begin() + index);

  auto dataTypeIndex = FindDataType(controller, typeid(T).name());
  if (dataTypeIndex != -1)
    controller->DataTypes.erase(controller->DataTypes.begin() + dataTypeIndex);
}
}; // namespace Tortuga

#endif
