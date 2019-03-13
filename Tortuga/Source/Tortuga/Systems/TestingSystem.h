#ifndef _TESTING_SYSTEM
#define _TESTING_SYSTEM

#include "../Console.h"
#include "../EntityData/MeshRenderer.h"
#include "../EntityData/RigidBody.h"
#include "../System.h"

namespace Tortuga
{
class TestingSystem : public System
{
  void OnStart()
  {
    //This system will need access to mesh renderer
    RegisterDataStructure<MeshRenderer>();
  }

  void OnUpdate()
  {
    //Get all the mesh renderer components in all
    //game objects (entities)
    auto data = GetData<MeshRenderer>();

    //Submit data to be updated before the
    //next frame starts
    SetData<MesRenderer>(data);
  }

  void OnEnd()
  {
    //Not Required
    UnRegisterDataStructure<MeshRenderer>();
  }
};
}; // namespace Tortuga

#endif