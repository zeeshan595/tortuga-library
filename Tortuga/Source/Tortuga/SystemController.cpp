#include "SystemController.h"

namespace Tortuga
{
void SystemThreadA(System<EntityDataStructure> *sys)
{
  sys->DataTransferLock.lock();
  sys->OnStart();
  sys->DataTransferLock.unlock();
  while (sys->SignalStop.load() == false)
  {
    sys->DataTransferLock.lock();
    sys->OnUpdate();
    sys->DataTransferLock.unlock();
  }
  sys->DataTransferLock.lock();
  sys->OnEnd();
  sys->DataTransferLock.unlock();
}
void SystemThreadB(System<EntityDataStructure, EntityDataStructure> *sys)
{
  sys->DataTransferLock.lock();
  sys->OnStart();
  sys->DataTransferLock.unlock();
  while (sys->SignalStop.load() == false)
  {
    sys->DataTransferLock.lock();
    sys->OnUpdate();
    sys->DataTransferLock.unlock();
  }
  sys->DataTransferLock.lock();
  sys->OnEnd();
  sys->DataTransferLock.unlock();
}
void SystemThreadC(System<EntityDataStructure, EntityDataStructure, EntityDataStructure> *sys)
{
  sys->DataTransferLock.lock();
  sys->OnStart();
  sys->DataTransferLock.unlock();
  while (sys->SignalStop.load() == false)
  {
    sys->DataTransferLock.lock();
    sys->OnUpdate();
    sys->DataTransferLock.unlock();
  }
  sys->DataTransferLock.lock();
  sys->OnEnd();
  sys->DataTransferLock.unlock();
}

SystemController *CreateSystemController()
{
  auto data = new SystemController();
  return data;
}
void DestroySystemController(SystemController *controller)
{
  for (uint32_t i = 0; i < controller->AttachedSystemsA.size(); i++)
  {
    controller->AttachedSystemsA[i]->SignalStop = true;
    controller->AttachedSystemsA[i]->ThreadHandle.join();
    delete controller->AttachedSystemsA[i];
  }
  for (uint32_t i = 0; i < controller->AttachedSystemsB.size(); i++)
  {
    controller->AttachedSystemsB[i]->SignalStop = true;
    controller->AttachedSystemsB[i]->ThreadHandle.join();
    delete controller->AttachedSystemsB[i];
  }
  for (uint32_t i = 0; i < controller->AttachedSystemsC.size(); i++)
  {
    controller->AttachedSystemsC[i]->SignalStop = true;
    controller->AttachedSystemsC[i]->ThreadHandle.join();
    delete controller->AttachedSystemsC[i];
  }

  controller->AttachedSystemsA.clear();
  controller->AttachedSystemsB.clear();
  controller->AttachedSystemsC.clear();
  delete controller;
}
void ProcessSystemController(SystemController *controller, Environment *env)
{
  for (uint32_t i = 0; i < controller->AttachedSystemsA.size(); i++)
  {
    controller->AttachedSystemsA[i]->DataTransferLock.lock();
    auto data = controller->AttachedSystemsA[i]->GetTypeInfos();
    controller->AttachedSystemsA[i]->PullData(
        {ExtractEntitiesDataStructures(env, data[0])});
    controller->AttachedSystemsA[i]->DataTransferLock.unlock();
  }
  for (uint32_t i = 0; i < controller->AttachedSystemsB.size(); i++)
  {
    controller->AttachedSystemsB[i]->DataTransferLock.lock();
    auto data = controller->AttachedSystemsB[i]->GetTypeInfos();
    controller->AttachedSystemsB[i]->PullData(
        {ExtractEntitiesDataStructures(env, data[0]),
         ExtractEntitiesDataStructures(env, data[1])});
    controller->AttachedSystemsB[i]->DataTransferLock.unlock();
  }
  for (uint32_t i = 0; i < controller->AttachedSystemsC.size(); i++)
  {
    controller->AttachedSystemsC[i]->DataTransferLock.lock();
    auto data = controller->AttachedSystemsC[i]->GetTypeInfos();
    controller->AttachedSystemsC[i]->PullData(
        {ExtractEntitiesDataStructures(env, data[0]),
         ExtractEntitiesDataStructures(env, data[1]),
         ExtractEntitiesDataStructures(env, data[2])});
    controller->AttachedSystemsC[i]->DataTransferLock.unlock();
  }
}
}; // namespace Tortuga