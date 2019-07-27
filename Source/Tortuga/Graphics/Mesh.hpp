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

const uint32_t MAX_VERTICES_SIZE = 65535;
const uint32_t MAX_INDICES_SIZE = 65535;
const uint32_t MESH_SIZE_IN_BYTES = ((sizeof(Vertex) * MAX_VERTICES_SIZE) +
                                     (sizeof(uint32_t) * MAX_INDICES_SIZE) +
                                     (sizeof(uint32_t) + sizeof(uint32_t)) +
                                     (sizeof(glm::vec3) + sizeof(float)));
struct Mesh
{
  Vertex Vertices[MAX_VERTICES_SIZE];
  uint32_t Indices[MAX_INDICES_SIZE];
  uint32_t VerticesSize;
  uint32_t IndicesSize;
  glm::vec3 BoundCenter;
  float BoundRadius;

  void SetIndices(std::vector<uint32_t> indices)
  {
    if (indices.size() > MAX_INDICES_SIZE)
    {
      Console::Warning("Indices size must be smaller than {0}", MAX_INDICES_SIZE);
      return;
    }
    memcpy(this->Indices, indices.data(), indices.size() * sizeof(uint32_t));
    this->IndicesSize = indices.size();
  }

  void SetVertices(std::vector<Vertex> vertices)
  {
    if (vertices.size() > MAX_VERTICES_SIZE)
    {
      Console::Warning("Vertices size must be smaller than {0}", MAX_VERTICES_SIZE);
      return;
    }
    memcpy(this->Vertices, vertices.data(), vertices.size() * sizeof(Vertex));
    this->VerticesSize = vertices.size();
  }
  void UpdateBounds()
  {
    glm::vec3 center;
    for (uint32_t i = 0; i < this->VerticesSize; i++)
      center += this->Vertices[i].Position;
    center = {center.x / this->VerticesSize, center.y / this->VerticesSize, center.z / this->VerticesSize};
    this->BoundCenter = center;
    
    float radius = 0.0f;
    for (uint32_t i = 0; i < this->VerticesSize; i++)
    {
      auto bound = glm::abs(this->BoundCenter - this->Vertices[i].Position).length();
      if (radius < bound)
        radius = bound;
    }
    this->BoundRadius = radius;
  }
};
} // namespace Graphics
} // namespace Tortuga

#endif