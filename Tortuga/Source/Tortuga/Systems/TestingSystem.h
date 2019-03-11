#ifndef _TESTING_SYSTEM
#define _TESTING_SYSTEM

#include "../Console.h"
#include "../EntityData/MeshRenderer.h"
#include "../SystemController.h"

namespace Tortuga
{
class TestingSystem : public System<MeshRenderer>
{
  void OnUpdate()
  {
    Console::Info("Example: {0}", (uint32_t)this->SceneData.size()); 
  }
};
};

#endif