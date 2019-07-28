#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "./Vulkan/DescriptorLayout.hpp"
#include "./Vulkan/DescriptorPool.hpp"
#include "./Vulkan/DescriptorSets.hpp"
#include "./Vulkan/Buffer.hpp"

#include "../Console.hpp"
#include "./Vertex.hpp"

namespace Tortuga
{
namespace Graphics
{
#define MAX_VERTICES_SIZE 65535
#define MAX_INDICES_SIZE 65535
const uint32_t MESH_SIZE = ((sizeof(Vertex) * MAX_VERTICES_SIZE) +
                            (sizeof(uint32_t) * MAX_INDICES_SIZE) +
                            (sizeof(uint32_t) + sizeof(glm::mat4)));

struct MeshBufferData
{
  uint32_t VerticesSize;
  uint32_t IndicesSize;
  Vertex Vertices[MAX_VERTICES_SIZE];
  uint32_t Indices[MAX_INDICES_SIZE];
  uint32_t IsDirty;
  glm::mat4x4 Transformation;
};
struct Mesh
{
  MeshBufferData BufferData;
  Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
  Vulkan::DescriptorSets::DescriptorSets DescriptorSets;
  Vulkan::Buffer::Buffer Staging;
  Vulkan::Buffer::Buffer Buffer;

  Mesh();
  ~Mesh();

  void ResetTransformation();
  void ApplyTransformation(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);
  void SetVertices(std::vector<Vertex> vertices);
  void SetIndices(std::vector<uint32_t> indices);
};
} // namespace Graphics
} // namespace Tortuga

#endif