#ifndef _TESTING_SYSTEM
#define _TESTING_SYSTEM

#include "../Console.h"
#include "../EntityData/MeshRenderer.h"
#include "../EntityData/RigidBody.h"
#include "../System.h"

namespace Tortuga
{
class TestingSystem : public System<MeshRenderer, RigidBody>
{
  void OnStart()
  {
  }

  void OnUpdate()
  {
    auto temp = SceneDataA;
    Console::Info("Size: {0}", (uint32_t)temp.size());
    //Returns all mesh renderers attached to all objects
    //1 object can only have 1 mesh renderer
    auto data = GetData<MeshRenderer>();

    //Do something with the RigidBodies
    //auto bodies = GetData<RigidBody>();
  }

  void OnEnd()
  {
  }
};
}; // namespace Tortuga

#endif