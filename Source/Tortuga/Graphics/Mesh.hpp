#ifndef _MESH
#define _MESH

#include <glm/glm.hpp>
#include <vector>

#include "../Console.hpp"
#include "./Vertex.hpp"

namespace Tortuga
{
namespace Graphics
{
struct Mesh
{
  std::vector<Vertex> Vertices;
  std::vector<uint32_t> Indices;
};
namespace MeshTransform
{
struct TransformedMesh
{
};
struct MeshTransform
{
};

MeshTransform Create();
void Destroy(MeshTransform data);
void Transform(MeshTransform data, Mesh mesh);
} // namespace MeshTransform
} // namespace Graphics
} // namespace Tortuga

#endif