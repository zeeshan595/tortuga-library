#include "SystemController.h"

namespace Tortuga
{
void SystemThread(System *ref)
{
  ref->OnStart();
  while (ref->IsSystemActive.load())
  {
    ref->OnUpdate();
  }
  ref->OnEnd();
}

SystemController CreateSystemController(Environment *environment)
{
  auto data = SystemController();
  {
    data.Scene = environment;
  }
  return data;
}
void DestroySystemController(SystemController controller)
{
  ClearSystems(controller);
}

void RemoveSystemAtPosition(SystemController &controller, uint32_t i)
{
  controller.Systems[i]->IsSystemActive = false;
  controller.Systems[i]->SystemThread.join();
  controller.Systems.erase(controller.Systems.begin() + i);
}

void ClearSystems(SystemController &controller)
{
  for (uint32_t i = 0; i < controller.Systems.size(); i++)
  {
    RemoveSystemAtPosition(controller, i);
  }
}
void ProcessSystemUpdate(SystemController &controller)
{
  for (uint32_t i = 0; i < controller.Systems.size(); i++)
  {
    controller.Systems[i]->OnMainThreadUpdate();
  }
}
}; // namespace Tortuga
