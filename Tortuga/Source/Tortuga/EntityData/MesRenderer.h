#ifndef _ENTITY_DATA_MESH_RENDERER
#define _ENTITY_DATA_MESH_RENDERER

#include "../Core.h"
#include "../Graphics/Vertex.h"
#include "../Environment.h"

namespace Tortuga
{
struct MeshRenderer : public EntityData
{
  std::vector<Graphics::Vertex> vertices;
  std::vector<uint16_t> indices;
};
}; // namespace Tortuga

#endif