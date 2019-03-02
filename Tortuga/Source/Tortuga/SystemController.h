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
  std::thread SystemThread;
  std::atomic<bool> IsSystemActive;
  Environment *Scene;

  System() {}
  ~System() {}

  //Calls on main thread before OnStart
  virtual void OnAwake() {}
  //Calls every frame on main thread
  virtual void OnMainThreadUpdate(){};

  //Calls when system is initialized
  virtual void OnStart() {}
  //Calls repeatedly
  virtual void OnUpdate() {}
  //Calls when system is removed/destroyed
  virtual void OnEnd() {}
};
struct SystemController
{
  Environment *Scene;
  std::vector<System *> Systems;
};
SystemController CreateSystemController(Environment *environment);
void DestroySystemController(SystemController controller);
void SystemThread(System *ref);
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
  
  //Setup important variables
  data->Scene = controller.Scene;
  data->IsSystemActive = true;
  
  data->OnAwake();
  data->SystemThread = std::thread(&SystemThread, data);
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