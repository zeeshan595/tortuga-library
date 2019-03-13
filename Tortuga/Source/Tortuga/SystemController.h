#ifndef _SYSTEM_CONTROLLER
#define _SYSTEM_CONTROLLER

#include "Core.h"
#include "Environment.h"
#include "System.h"

namespace Tortuga
{
struct SystemController
{
  std::vector<System *> AttachedSystems;
};

SystemController *CreateSystemController();
void DestroySystemController(SystemController *controller);
void ProcessSystemController(SystemController *controller, Environment *env);
void SystemThreadA(System *sys);
template <typename T>
int32_t FindSystem(SystemController *controller)
{
  for (uint32_t i = 0; i < controller->AttachedSystems.size(); i++)
  {
    auto data = dynamic_cast<T *>(controller->AttachedSystems[i]);
    if (data != nullptr)
      return i;
  }
  return -1;
}

template <typename T>
void AddSystem(SystemController *controller)
{
  auto data = (System *)(new T());
  if (data == nullptr)
    return;

  data->OnStart();
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
}
}; // namespace Tortuga

#endif
