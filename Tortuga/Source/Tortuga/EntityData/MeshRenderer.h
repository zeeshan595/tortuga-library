#ifndef _MESH_RENDERER
#define _MESH_RENDERER

#include "../Entity.h"

namespace Tortuga
{
struct MeshRenderer : public EntityDataStructure
{
  MeshRenderer() : 
  EntityDataStructure(std::string(typeid(MeshRenderer).name()))
  {
 
  }
};
}; // namespace Tortuga

#endif