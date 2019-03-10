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
  delete controller;
}
void ProcessSystemController(SystemController *controller)
{
  for (uint32_t i = 0; i < controller->AttachedSystems.size(); i++)
  {
    controller->AttachedSystems[i]->OnUpdate();
  }
}
}; // namespace Tortuga