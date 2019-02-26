#include "SystemController.h"

namespace Tortuga
{
int SystemThread(void *ptr)
{
  System *ref = (System *)ptr;

  ref->OnStart();
  bool isThreadActive = true;
  while (isThreadActive)
  {
    ref->OnUpdate();

    if (SDL_LockMutex(ref->SystemMutex) == 0)
    {
      isThreadActive = ref->IsSystemActive;
      SDL_UnlockMutex(ref->SystemMutex);
    }
  }
  ref->OnEnd();

  return 0;
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
  if (SDL_LockMutex(controller.Systems[i]->SystemMutex) == 0)
  {
    controller.Systems[i]->IsSystemActive = false;
    SDL_UnlockMutex(controller.Systems[i]->SystemMutex);
  }
  int threadReturnValue;
  SDL_WaitThread(controller.Systems[i]->SystemThread, &threadReturnValue);
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
