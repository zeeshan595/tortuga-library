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
  for (uint32_t i = 0; i < controller->AttachedSystems.size(); i++)
  {
    controller->AttachedSystems[i]->OnEnd();
    delete controller->AttachedSystems[i];
  }

  controller->AttachedSystems.clear();
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
  for (uint32_t i = 0; i < controller->AttachedSystems.size(); i++)
  {
    auto data = controller->AttachedSystems[i]->GetTypeInfos();
    switch (controller->AttachedSystems[i]->GetStructureType())
    {
    case SYSTEM_STRUCTURE_TYPE_A:
      controller->AttachedSystems[i]->PullData(
          {ExtractEntitiesDataStructures(env, data[0])});
      break;
    case SYSTEM_STRUCTURE_TYPE_B:
      controller->AttachedSystems[i]->PullData(
          {ExtractEntitiesDataStructures(env, data[0]),
           ExtractEntitiesDataStructures(env, data[1])});
      break;
    case SYSTEM_STRUCTURE_TYPE_C:
      controller->AttachedSystems[i]->PullData(
          {ExtractEntitiesDataStructures(env, data[0]),
           ExtractEntitiesDataStructures(env, data[1]),
           ExtractEntitiesDataStructures(env, data[2])});
      break;
    default:
      break;
    }

    controller->AttachedSystems[i]->OnUpdate();
  }
}
}; // namespace Tortuga