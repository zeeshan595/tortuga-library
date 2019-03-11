#include "SystemController.h"

namespace Tortuga
{
SystemController *CreateSystemController()
{
  auto data = new SystemController();
  return data;
}
void DestroySystemController(SystemController *controller)
{
  for (uint32_t i = 0; i < controller->AttachedSystemsA.size(); i++)
  {
    controller->AttachedSystemsA[i]->OnEnd();
    delete controller->AttachedSystemsA[i];
  }
  for (uint32_t i = 0; i < controller->AttachedSystemsB.size(); i++)
  {
    controller->AttachedSystemsB[i]->OnEnd();
    delete controller->AttachedSystemsB[i];
  }
  for (uint32_t i = 0; i < controller->AttachedSystemsC.size(); i++)
  {
    controller->AttachedSystemsC[i]->OnEnd();
    delete controller->AttachedSystemsC[i];
  }

  controller->AttachedSystemsA.clear();
  controller->AttachedSystemsB.clear();
  controller->AttachedSystemsC.clear();
  controller->DataTypes.clear();
  delete controller;
}
int32_t FindDataType(SystemController *controller, std::string typeInfo)
{
  for (uint32_t i = 0; i < controller->DataTypes.size(); i++)
  {
    if (controller->DataTypes[i] == typeInfo)
      return i;
  }

  return -1;
}
void ProcessSystemController(SystemController *controller, Environment *env)
{
  for (uint32_t i = 0; i < controller->AttachedSystemsA.size(); i++)
  {
    auto data = controller->AttachedSystemsA[i]->GetTypeInfos();
    controller->AttachedSystemsA[i]->PullData(
        {ExtractEntitiesDataStructures(env, data[0])});

    controller->AttachedSystemsA[i]->OnUpdate();
  }

  for (uint32_t i = 0; i < controller->AttachedSystemsB.size(); i++)
  {
    auto data = controller->AttachedSystemsB[i]->GetTypeInfos();
    controller->AttachedSystemsB[i]->PullData(
        {ExtractEntitiesDataStructures(env, data[0]),
         ExtractEntitiesDataStructures(env, data[1])});

    controller->AttachedSystemsB[i]->OnUpdate();
  }

  for (uint32_t i = 0; i < controller->AttachedSystemsC.size(); i++)
  {
    auto data = controller->AttachedSystemsC[i]->GetTypeInfos();
    controller->AttachedSystemsC[i]->PullData(
        {ExtractEntitiesDataStructures(env, data[0]),
         ExtractEntitiesDataStructures(env, data[1]),
         ExtractEntitiesDataStructures(env, data[2])});

    controller->AttachedSystemsC[i]->OnUpdate();
  }
}
}; // namespace Tortuga