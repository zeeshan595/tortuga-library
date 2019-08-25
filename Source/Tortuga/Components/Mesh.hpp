#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "../Graphics/Vulkan/DescriptorLayout.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"

#include "../Core/Engine.hpp"
#include "../Console.hpp"
#include "../Graphics/Index.hpp"

namespace Tortuga
{
namespace Component
{
#define MAX_VERTICES_SIZE 65535
#define MAX_INDICES_SIZE 65535

struct MeshBufferData
{
  uint32_t VerticesSize;
  uint32_t NormalsSize;
  uint32_t TexturesSize;
  uint32_t IndicesSize;
  glm::vec4 Center;
  glm::mat4 Transformation;
  glm::mat4 NormalMatrix;
  glm::vec4 Vertices[MAX_VERTICES_SIZE];
  glm::vec4 Normals[MAX_VERTICES_SIZE];
  glm::vec2 Textures[MAX_VERTICES_SIZE];
  Graphics::Index Indices[MAX_INDICES_SIZE];
  float Bounds;
};
const uint32_t MESH_SIZE = sizeof(MeshBufferData);
struct Mesh
{
  bool IsEnabled;
  bool IsStatic;
  bool IsProcessedOnce;
  MeshBufferData BufferData;
  Graphics::Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet DescriptorSets;
  Graphics::Vulkan::Buffer::Buffer Staging;
  Graphics::Vulkan::Buffer::Buffer Buffer;
  Graphics::Vulkan::CommandPool::CommandPool CommandPool;
  Graphics::Vulkan::Command::Command Command;

  void ResetTransformation();
  void ApplyTransformation(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale);
  void SetVertices(std::vector<glm::vec4> vertices, bool recalculateBounds = false);
  void SetNormals(std::vector<glm::vec4> normals);
  void SetTextures(std::vector<glm::vec4> textures);
  void SetIndices(std::vector<Graphics::Index> indices);
  void SetStatic(bool isStatic);
  void SetActive(bool isActive);

  Mesh();
  ~Mesh();
};
Graphics::Vulkan::DescriptorLayout::DescriptorLayout GetMeshDescriptorLayout();
} // namespace Component
} // namespace Tortuga

#endif