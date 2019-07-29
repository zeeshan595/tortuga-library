#ifndef _SYSTEM
#define _SYSTEM

#include "unordered_map"
#include "typeindex"
#include <thread>
#include <future>

#include "./Entity.hpp"

namespace Tortuga
{
namespace Core
{
template <typename T>
struct ComponentData
{
  Entity::Entity *Entity;
  T *Data;
};
class System
{
protected:
  template <typename T>
  std::vector<ComponentData<T>> GetData()
  {
    auto entities = Entity::GetAllEntities();
    std::vector<ComponentData<T>> data(entities.size());
    for (uint32_t i = 0; i < entities.size(); i++)
    {
      data[i].Entity = entities[i];
      data[i].Data = entities[i]->GetComponent<T>();
    }
    return data;
  }

public:
  virtual void Start() {}
  virtual void Update() {}
  virtual void End() {}
};
struct SystemWrapper
{
  System *SystemPtr;
  std::thread SystemThread;
  std::promise<void> *ExitSignal;
};
void SystemThread(System *system, std::future<void> cancellation);
std::unordered_map<std::type_index, SystemWrapper *> SystemManager;
template <typename T>
T *CreateSystem()
{
  auto type = std::type_index(typeid(T));
  if (SystemManager[type] != nullptr)
    return static_cast<T *>(SystemManager[type]->SystemPtr);

  auto wrapper = new SystemWrapper();
  {
    wrapper->SystemPtr = static_cast<System *>(new T());
    wrapper->ExitSignal = new std::promise<void>();
    wrapper->SystemThread = std::thread(SystemThread, wrapper->SystemPtr, std::move((*wrapper->ExitSignal).get_future()));
  }

  SystemManager[type] = wrapper;
  return static_cast<T *>(SystemManager[type]->SystemPtr);
}
template <typename T>
void DestroySystem()
{
  auto type = std::type_index(typeid(T));
  if (SystemManager[type] == nullptr)
    return;

  SystemManager[type]->ExitSignal->set_value();
  SystemManager[type]->SystemThread.join();
  delete SystemManager[type];
  SystemManager.erase(type);
}

void SystemThread(System *system, std::future<void> cancellation)
{
  system->Start();
  while (cancellation.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    system->Update();
  system->End();
}
} // namespace Core
} // namespace Tortuga

#endif