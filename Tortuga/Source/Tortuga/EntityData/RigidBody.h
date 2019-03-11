#ifndef _RIGID_BODY
#define _RIGID_BODY

#include "../Entity.h"

namespace Tortuga
{
struct RigidBody : public EntityDataStructure
{
  RigidBody() : 
  EntityDataStructure(std::string(typeid(RigidBody).name()))
  {
 
  }
};
};

#endif