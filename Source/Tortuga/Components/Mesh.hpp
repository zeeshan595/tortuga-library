#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "../Graphics/Vulkan/DescriptorLayout.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSets.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"

#include "../Core/Engine.hpp"
#include "../Console.hpp"
#include "../Graphics/Vertex.hpp"

namespace Tortuga
{
namespace Component
{
#define MAX_VERTICES_SIZE 3
#define MAX_INDICES_SIZE 3
const uint32_t MESH_SIZE = ((sizeof(glm::mat4) + sizeof(glm::mat4)) +
                            (sizeof(Graphics::Vertex) * MAX_VERTICES_SIZE) +
                            (sizeof(int32_t) * MAX_INDICES_SIZE) +
                            (sizeof(int32_t) + sizeof(int32_t)));

struct MeshBufferData
{
  int32_t VerticesSize;
  int32_t IndicesSize;
  glm::mat4 Transformation;
  glm::mat4 NormalMatrix;
  Graphics::Vertex Vertices[MAX_VERTICES_SIZE];
  int32_t Indices[MAX_INDICES_SIZE];
};
struct Mesh
{
  MeshBufferData BufferData;
  Graphics::Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
  Graphics::Vulkan::DescriptorSets::DescriptorSets DescriptorSets;
  Graphics::Vulkan::Buffer::Buffer Staging;
  Graphics::Vulkan::Buffer::Buffer Buffer;
  Graphics::Vulkan::CommandPool::CommandPool CommandPool;
  Graphics::Vulkan::Command::Command Command;

  void ResetTransformation();
  void ApplyTransformation(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);
  void SetVertices(std::vector<Graphics::Vertex> vertices);
  void SetIndices(std::vector<uint32_t> indices);

  Mesh();
  ~Mesh();
};
Graphics::Vulkan::DescriptorLayout::DescriptorLayout GetMeshDescriptorLayout();
} // namespace Component
} // namespace Tortuga

#endif