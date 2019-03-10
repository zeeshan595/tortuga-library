#ifndef _SYSTEM_CONTROLLER
#define _SYSTEM_CONTROLLER

#include "Core.h"
#include "Environment.h"

namespace Tortuga
{
template <typename T>
struct System
{
  static_assert(std::is_base_of<EntityDataStructure, T>::value, "T must inherit from Key");

  std::vector<EntityExtractedData<T>> SceneData;

  virtual void OnStart() {}
  virtual void OnUpdate() {}
  virtual void OnEnd() {}

  virtual ~System() {}
};
struct SystemController
{
  std::vector<System<EntityDataStructure> *> AttachedSystems;
};

SystemController *CreateSystemController();
void DestroySystemController(SystemController *controller);
void ProcessSystemController(SystemController *controller);

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
