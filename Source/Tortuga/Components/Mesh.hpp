#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "../Graphics/Vertex.hpp"

namespace Tortuga
{
namespace Component
{
struct Mesh
{
  std::vector<Graphics::Vertex> Vertices;
  std::vector<uint32_t> Indices;
};
} // namespace Component
} // namespace Tortuga

#endif