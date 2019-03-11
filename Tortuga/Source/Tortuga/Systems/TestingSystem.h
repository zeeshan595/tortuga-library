#ifndef _TESTING_SYSTEM
#define _TESTING_SYSTEM

#include "../Console.h"
#include "../EntityData/MeshRenderer.h"
#include "../System.h"

namespace Tortuga
{
class TestingSystem : public System<MeshRenderer>
{
  void OnUpdate()
  {
  }
};
};

#endif