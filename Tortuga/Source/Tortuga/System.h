#ifndef _SYSTEMS
#define _SYSTEMS

#include "Entity.h"

namespace Tortuga
{
template <typename...>
struct System;

enum SystemStructureType
{
  SYSTEM_STRUCTURE_TYPE_A,
  SYSTEM_STRUCTURE_TYPE_B,
  SYSTEM_STRUCTURE_TYPE_C
};

//===
//ONE
//===
template <typename A>
struct System<A>
{
  static_assert(std::is_base_of<EntityDataStructure, A>::value, "A must inherit from Key");

private:
  std::string TypeInfoA;
  const SystemStructureType Type = SYSTEM_STRUCTURE_TYPE_A;

public:
  std::vector<EntityExtractedData<A>> SceneDataA;
  void PullData(std::vector<std::vector<EntityExtractedData<EntityDataStructure>>> data)
  {
    SceneDataA.resize(data[0].size());
    for (uint32_t i = 0; i < data[0].size(); i++)
    {
      SceneDataA[i] = data[0][i];
    }
  }

  SystemStructureType GetStructureType() { return Type; }
  std::vector<std::string> GetTypeInfos() { return {TypeInfoA}; }

  virtual void OnStart() {}
  virtual void OnUpdate() {}
  virtual void OnEnd() {}

  System()
  {
    TypeInfoA = typeid(A).name();
  }
  virtual ~System() {}
};

//===
//TWO
//===
template <typename A, typename B>
struct System<A, B>
{
  static_assert(std::is_base_of<EntityDataStructure, A>::value, "A must inherit from Key");
  static_assert(std::is_base_of<EntityDataStructure, B>::value, "B must inherit from Key");

private:
  std::string TypeInfoA;
  std::string TypeInfoB;
  const SystemStructureType Type = SYSTEM_STRUCTURE_TYPE_B;

public:
  std::vector<EntityExtractedData<A>> SceneDataA;
  std::vector<EntityExtractedData<B>> SceneDataB;
  void PullData(std::vector<std::vector<EntityExtractedData<EntityDataStructure>>> data)
  {
    SceneDataA.resize(data[0].size());
    for (uint32_t i = 0; i < data[0].size(); i++)
    {
      SceneDataA[i] = data[0][i];
    }

    SceneDataB.resize(data[1].size());
    for (uint32_t i = 0; i < data[1].size(); i++)
    {
      SceneDataB[i] = data[1][i];
    }
  }

  SystemStructureType GetStructureType() { return Type; }
  std::vector<std::string> GetTypeInfos() { return {TypeInfoA, TypeInfoB}; }

  virtual void OnStart() {}
  virtual void OnUpdate() {}
  virtual void OnEnd() {}

  System()
  {
    TypeInfoA = typeid(A).name();
    TypeInfoB = typeid(B).name();
  }
  virtual ~System() {}
};

//=====
//THREE
//=====
template <typename A, typename B, typename C>
struct System<A, B, C>
{
  static_assert(std::is_base_of<EntityDataStructure, A>::value, "A must inherit from Key");
  static_assert(std::is_base_of<EntityDataStructure, B>::value, "B must inherit from Key");
  static_assert(std::is_base_of<EntityDataStructure, C>::value, "C must inherit from Key");

private:
  std::string TypeInfoA;
  std::string TypeInfoB;
  std::string TypeInfoC;
  const SystemStructureType Type = SYSTEM_STRUCTURE_TYPE_C;

public:
  std::vector<EntityExtractedData<A>> SceneDataA;
  std::vector<EntityExtractedData<B>> SceneDataB;
  std::vector<EntityExtractedData<C>> SceneDataC;
  void PullData(std::vector<std::vector<EntityExtractedData<EntityDataStructure>>> data)
  {
    SceneDataA.resize(data[0].size());
    for (uint32_t i = 0; i < data[0].size(); i++)
    {
      SceneDataA[i] = data[0][i];
    }

    SceneDataB.resize(data[1].size());
    for (uint32_t i = 0; i < data[1].size(); i++)
    {
      SceneDataB[i] = data[1][i];
    }

    SceneDataC.resize(data[2].size());
    for (uint32_t i = 0; i < data[2].size(); i++)
    {
      SceneDataC[i] = data[2][i];
    }
  }

  SystemStructureType GetStructureType() { return Type; }
  std::vector<std::string> GetTypeInfos() { return {TypeInfoA, TypeInfoB, TypeInfoC}; }

  virtual void OnStart() {}
  virtual void OnUpdate() {}
  virtual void OnEnd() {}

  System()
  {
    TypeInfoA = typeid(A).name();
    TypeInfoB = typeid(B).name();
    TypeInfoC = typeid(C).name();
  }
  virtual ~System() {}
};
}; // namespace Tortuga

#endif