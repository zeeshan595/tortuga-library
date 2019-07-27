#ifndef _MESH
#define _MESH

#include <glm/glm.hpp>
#include <vector>

#include "./Vertex.hpp"

namespace Tortuga
{
namespace Graphics
{

const uint32_t MAX_VERTICES_SIZE = 65535;
const uint32_t MAX_INDICES_SIZE = 65535;
const uint32_t MESH_SIZE_IN_BYTES = ((sizeof(Vertex) * MAX_VERTICES_SIZE) +
                                     (sizeof(uint32_t) * MAX_INDICES_SIZE) +
                                     (sizeof(glm::vec3) + sizeof(uint32_t)));
struct Mesh
{
  Vertex Vertices[MAX_VERTICES_SIZE];
  uint32_t Indices[MAX_INDICES_SIZE];
  glm::vec3 boundCenter;
  uint32_t boundRadius;
};
} // namespace Graphics
} // namespace Tortuga

#endif