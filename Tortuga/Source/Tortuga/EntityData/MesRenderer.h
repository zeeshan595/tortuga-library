#ifndef _ENTITY_DATA_MESH_RENDERER
#define _ENTITY_DATA_MESH_RENDERER

#include "../Core.h"
#include "../Primitive.h"

namespace Tortuga
{

struct MeshRenderer : public DataStructure
{
  bool IsEnabled = true;
  std::vector<Primitive> Primitives = {
      Primitive()};
};
}; // namespace Tortuga

#endif