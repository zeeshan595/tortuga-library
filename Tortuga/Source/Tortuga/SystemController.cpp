#include "SystemController.h"

namespace Tortuga
{
void SystemThread(System *sys)
{
}

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
  delete controller;
}
void ProcessSystemController(SystemController *controller, Environment *env)
{
  for (uint32_t i = 0; i < controller->AttachedSystems.size(); i++)
  {
    auto infos = controller->AttachedSystems[i]->GetTypeInfo();
    for (uint32_t j = 0; j < infos.size(); j++)
    {
      PackEntityDataStructures(env, controller->AttachedSystems[i]->PollData(infos[j]));
      auto scene = ExtractEntitiesDataStructures(env, infos[j]);
      controller->AttachedSystems[i]->PushSceneData(infos[j], scene);
    }
    controller->AttachedSystems[i]->OnUpdate();
  }
}
}; // namespace Tortuga