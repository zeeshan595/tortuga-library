#ifndef _SYSTEM_CONTROLLER
#define _SYSTEM_CONTROLLER

#include "Core.h"
#include "Environment.h"
#include "System.h"

namespace Tortuga
{
struct SystemController
{
  std::vector<System<EntityDataStructure> *> AttachedSystemsA;
  std::vector<System<EntityDataStructure, EntityDataStructure> *> AttachedSystemsB;
  std::vector<System<EntityDataStructure, EntityDataStructure, EntityDataStructure> *> AttachedSystemsC;
};

SystemController *CreateSystemController();
void DestroySystemController(SystemController *controller);
void ProcessSystemController(SystemController *controller, Environment *env);
void SystemThreadA(System<EntityDataStructure> *sys);
void SystemThreadB(System<EntityDataStructure, EntityDataStructure> *sys);
void SystemThreadC(System<EntityDataStructure, EntityDataStructure, EntityDataStructure> *sys);

template <typename T>
int32_t FindSystem(SystemController *controller, SystemStructureType &StructureType)
{
  StructureType = SYSTEM_STRUCTURE_TYPE_A;
  for (uint32_t i = 0; i < controller->AttachedSystemsA.size(); i++)
  {
    auto data = (System<T>)controller->AttachedSystemsA[i];
    if (data != nullptr)
      return i;
  }
  StructureType = SYSTEM_STRUCTURE_TYPE_B;
  for (uint32_t i = 0; i < controller->AttachedSystemsB.size(); i++)
  {
    auto data = (System<T>)controller->AttachedSystemsB[i];
    if (data != nullptr)
      return i;
  }
  StructureType = SYSTEM_STRUCTURE_TYPE_C;
  for (uint32_t i = 0; i < controller->AttachedSystemsC.size(); i++)
  {
    auto data = (System<T>)controller->AttachedSystemsC[i];
    if (data != nullptr)
      return i;
  }
  return -1;
}

template <typename T>
void AddSystem(SystemController *controller)
{
  switch (T().GetStructureType())
  {
  case SYSTEM_STRUCTURE_TYPE_A:
  {
    auto data = (System<EntityDataStructure> *)(new T());
    if (data == nullptr)
      return;

    data->SignalStop.store(false);
    data->ThreadHandle = std::thread(SystemThreadA, data);
    controller->AttachedSystemsA.push_back(data);
  }
  break;
  case SYSTEM_STRUCTURE_TYPE_B:
  {
    auto data = (System<EntityDataStructure, EntityDataStructure> *)(new T());
    if (data == nullptr)
      return;

    data->SignalStop.store(false);
    data->ThreadHandle = std::thread(SystemThreadB, data);
    controller->AttachedSystemsB.push_back(data);
  }
  break;
  case SYSTEM_STRUCTURE_TYPE_C:
  {
    auto data = (System<EntityDataStructure, EntityDataStructure, EntityDataStructure> *)(new T());
    if (data == nullptr)
      return;

    data->SignalStop.store(false);
    data->ThreadHandle = std::thread(SystemThreadC, data);
    controller->AttachedSystemsC.push_back(data);
  }
  break;
  default:
    Console::Error("Unknown System Given!");
    break;
  }
}

template <typename T>
void RemoveSystem(SystemController *controller)
{
  SystemStructureType SystemType;
  auto index = FindSystem<T>(controller, &SystemType);
  if (index == -1)
    return;

  switch (SystemType)
  {
  case SYSTEM_STRUCTURE_TYPE_A:
  {
    controller->AttachedSystemsA[index]->SignalStop.store(true);
    controller->AttachedSystemsA[index]->ThreadHandle.join();
    delete controller->AttachedSystemsA[index];
    controller->AttachedSystemsA.erase(controller->AttachedSystemsA.begin() + index);
  }
  break;
  case SYSTEM_STRUCTURE_TYPE_B:
  {
    controller->AttachedSystemsB[index]->SignalStop.store(true);
    controller->AttachedSystemsB[index]->ThreadHandle.join();
    delete controller->AttachedSystemsB[index];
    controller->AttachedSystemsB.erase(controller->AttachedSystemsB.begin() + index);
  }
  break;
  case SYSTEM_STRUCTURE_TYPE_C:
  {
    controller->AttachedSystemsC[index]->SignalStop.store(true);
    controller->AttachedSystemsC[index]->ThreadHandle.join();
    delete controller->AttachedSystemsC[index];
    controller->AttachedSystemsC.erase(controller->AttachedSystemsC.begin() + index);
  }
  break;
  }
}
}; // namespace Tortuga

#endif
