#ifndef _SYSTEM_CONTROLLER
#define _SYSTEM_CONTROLLER

#include "Core.h"
#include "Console.h"
#include "Environment.h"

namespace Tortuga
{
class System
{
public:
  SDL_mutex *SystemMutex;
  SDL_Thread *SystemThread;
  bool IsSystemActive;
  Environment *Scene;

  System() {}
  ~System() {}

  //Calls once when initialized
  virtual void OnStart() {}
  //Calls repeatedly
  virtual void OnUpdate() {}
  //Calls before system is destroyed
  virtual void OnEnd() {}
  //Calls every frame on main thread
  virtual void OnMainThreadUpdate(){};
};
struct SystemController
{
  Environment *Scene;
  std::vector<System *> Systems;
};
SystemController CreateSystemController(Environment *environment);
void DestroySystemController(SystemController controller);
int SystemThread(void *ptr);
void RemoveSystemAtPosition(SystemController &controller, uint32_t o);
void ClearSystems(SystemController &controller);
void ProcessSystemUpdate(SystemController &controller);

template <typename T>
void AddSystem(SystemController &controller)
{
  T *temp = new T();
  System *data = dynamic_cast<System *>(temp);
  if (data == nullptr)
  {
    Console::Error("All systems must inherit from the 'System' class!");
    return;
  }

  data->IsSystemActive = true;
  data->Scene = controller.Scene;
  data->SystemMutex = SDL_CreateMutex();
  data->SystemThread = SDL_CreateThread(SystemThread, "System Thread", (void *)data);
  if (data->SystemThread == nullptr)
  {
    Console::Error("Failed to create system thread!");
    return;
  }
  controller.Systems.push_back(data);
}
template <typename T>
void RemoveSystem(SystemController &controller)
{
  for (uint32_t i = 0; i < controller.Systems.size(); i++)
  {
    if (dynamic_cast<T *>(controller.Systems[i]) != nullptr)
    {
      RemoveSystemAtPosition(controller, i);
      break;
    }
  }
}
}; // namespace Tortuga

#endif