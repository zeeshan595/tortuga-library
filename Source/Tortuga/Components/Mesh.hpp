#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "../Graphics/Vulkan/Device.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Core/Entity.hpp"

namespace Tortuga
{
namespace Component
{
struct Mesh
{
private:
  std::vector<Core::Entity::Entity *> Lights;
  std::vector<Graphics::Vertex> Vertices;
  std::vector<uint32_t> Indices;
  uint32_t VerticesByteSize = 0;
  uint32_t IndicesByteSize = 0;

public:
  //gpu commands
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool RenderCommandPool;
  Graphics::Vulkan::Command::Command RenderCommand;
  Graphics::Vulkan::Command::Command TransferCommand;
  //mesh information
  Graphics::Vulkan::Buffer::Buffer StagingVertexBuffer;
  Graphics::Vulkan::Buffer::Buffer VertexBuffer;
  Graphics::Vulkan::Buffer::Buffer StagingIndexBuffer;
  Graphics::Vulkan::Buffer::Buffer IndexBuffer;
  //uniform buffer
  std::vector<Graphics::Vulkan::DescriptorPool::DescriptorPool> UniformBufferPool;
  std::vector<Graphics::Vulkan::DescriptorSet::DescriptorSet> UniformBufferSets;
  std::vector<Graphics::Vulkan::Buffer::Buffer> StagingUniformBuffer;
  std::vector<Graphics::Vulkan::Buffer::Buffer> UniformBuffer;
  //light information
  Graphics::Vulkan::DescriptorPool::DescriptorPool LightBufferPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet LightBufferSets;
  Graphics::Vulkan::Buffer::Buffer StagingLightBuffer;
  Graphics::Vulkan::Buffer::Buffer LightBuffer;

  ~Mesh()
  {
    Graphics::Vulkan::Device::WaitForDevice(Core::Engine::GetMainDevice());

    Graphics::Vulkan::CommandPool::Destroy(RenderCommandPool);
    Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);

    //mesh information
    Graphics::Vulkan::Buffer::Destroy(StagingVertexBuffer);
    Graphics::Vulkan::Buffer::Destroy(VertexBuffer);
    Graphics::Vulkan::Buffer::Destroy(StagingIndexBuffer);
    Graphics::Vulkan::Buffer::Destroy(IndexBuffer);

    //uniform buffer
    for (auto pool : UniformBufferPool)
      Graphics::Vulkan::DescriptorPool::Destroy(pool);
    for (auto buffer : StagingUniformBuffer)
      Graphics::Vulkan::Buffer::Destroy(buffer);
    for (auto buffer : UniformBuffer)
      Graphics::Vulkan::Buffer::Destroy(buffer);

    //light information
    Graphics::Vulkan::DescriptorPool::Destroy(LightBufferPool);
    Graphics::Vulkan::Buffer::Destroy(StagingLightBuffer);
    Graphics::Vulkan::Buffer::Destroy(LightBuffer);
  }

  void SetVertices(std::vector<Graphics::Vertex> vertices)
  {
    Vertices = vertices;
    VerticesByteSize = vertices.size() * sizeof(Graphics::Vertex);
  }

  void SetIndices(std::vector<uint32_t> indices)
  {
    Indices = indices;
    IndicesByteSize = indices.size() * sizeof(uint32_t);
  }

  void SetLights(std::vector<Core::Entity::Entity *> lights)
  {
    Lights = lights;
  }

  std::vector<Graphics::Vertex> GetVertices()
  {
    return Vertices;
  }
  std::vector<uint32_t> GetIndices()
  {
    return Indices;
  }
  std::vector<Core::Entity::Entity *> GetLights()
  {
    return Lights;
  }
  uint32_t GetVerticesByteSize()
  {
    return VerticesByteSize;
  }
  uint32_t GetIndicesByteSize()
  {
    return IndicesByteSize;
  }
};
} // namespace Component
} // namespace Tortuga

#endif