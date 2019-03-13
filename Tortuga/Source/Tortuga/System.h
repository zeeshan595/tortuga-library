#ifndef _SYSTEMS
#define _SYSTEMS

#include "Entity.h"

namespace Tortuga
{
struct System
{
private:
  std::unordered_map<std::type_index, std::vector<EntityExtractedData<EntityDataStructure>>> SceneData;
  std::vector<std::type_index> TypeInfos;

  template <typename T>
  int32_t FindTypeInfo()
  {
    for (uint32_t i = 0; i < TypeInfos.size(); i++)
    {
      if (TypeInfos[i] == std::type_index(typeid(T)))
        return i;
    }

    return -1;
  }

protected:
  template <typename T>
  void RegisterDataStructure()
  {
    if (FindTypeInfo<T>() != -1)
    {
      Console::Error("This Data structure has already been registered!");
      return;
    }

    auto info = std::type_index(typeid(T));
    TypeInfos.push_back(info);
  }
  template <typename T>
  void UnRegisterDataStructure()
  {
    auto index = FindTypeInfo<T>();
    if (index == -1)
      return;
    TypeInfos.erase(TypeInfos.begin() + index);
  }
  template <typename T>
  std::vector<EntityExtractedData<T>> GetData()
  {
    auto baseData = SceneData[std::type_index(typeid(T))];
    auto convertedData = reinterpret_cast<std::vector<EntityExtractedData<T>> *>(&baseData);
    return (*convertedData);
  }
  template <typename T>
  void SetData(std::vector<EntityExtractedData<T>> data)
  {
    SceneData[std::type_index(typeid(T))] = data;
  }

public:
  std::vector<std::type_index> GetTypeInfo()
  {
    return TypeInfos;
  }
  void PushSceneData(std::type_index info, std::vector<EntityExtractedData<EntityDataStructure>> data)
  {
    SceneData[info] = data;
  }
  std::vector<EntityExtractedData<EntityDataStructure>> PollData(std::type_index info)
  {
    return SceneData[info];
  }

  virtual void OnStart() {}
  virtual void OnUpdate() {}
  virtual void OnEnd() {}
};
}; // namespace Tortuga

#endif