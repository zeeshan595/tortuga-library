#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "../Graphics/Vulkan/Device.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vertex.hpp"

namespace Tortuga
{
namespace Component
{
struct Mesh
{
private:
  std::vector<Graphics::Vertex> Vertices;
  std::vector<uint32_t> Indices;
  uint32_t VerticesByteSize = 0;
  uint32_t IndicesByteSize = 0;

public:
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool RenderCommandPool;
  Graphics::Vulkan::Command::Command RenderCommand;
  Graphics::Vulkan::Command::Command TransferCommand;
  Graphics::Vulkan::Buffer::Buffer StagingVertexBuffer;
  Graphics::Vulkan::Buffer::Buffer VertexBuffer;
  Graphics::Vulkan::Buffer::Buffer StagingIndexBuffer;
  Graphics::Vulkan::Buffer::Buffer IndexBuffer;
  std::vector<Graphics::Vulkan::DescriptorPool::DescriptorPool> UniformBufferPool;
  std::vector<Graphics::Vulkan::DescriptorSet::DescriptorSet> UniformBufferSets;
  std::vector<Graphics::Vulkan::Buffer::Buffer> StagingUniformBuffer;
  std::vector<Graphics::Vulkan::Buffer::Buffer> UniformBuffer;

  Mesh()
  {
    TransferCommandPool.CommandPool = VK_NULL_HANDLE;
    RenderCommandPool.CommandPool = VK_NULL_HANDLE;
    TransferCommand.Command = VK_NULL_HANDLE;
    RenderCommand.Command = VK_NULL_HANDLE;
    StagingVertexBuffer.Buffer = VK_NULL_HANDLE;
    StagingIndexBuffer.Buffer = VK_NULL_HANDLE;
  }

  ~Mesh()
  {
    Graphics::Vulkan::Device::WaitForDevice(Core::Engine::GetMainDevice());

    if (RenderCommandPool.CommandPool != VK_NULL_HANDLE)
      Graphics::Vulkan::CommandPool::Destroy(RenderCommandPool);

    if (TransferCommandPool.CommandPool != VK_NULL_HANDLE)
      Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);

    if (StagingVertexBuffer.Buffer != VK_NULL_HANDLE)
      Graphics::Vulkan::Buffer::Destroy(StagingVertexBuffer);

    if (VertexBuffer.Buffer != VK_NULL_HANDLE)
      Graphics::Vulkan::Buffer::Destroy(VertexBuffer);

    if (StagingIndexBuffer.Buffer != VK_NULL_HANDLE)
      Graphics::Vulkan::Buffer::Destroy(StagingIndexBuffer);

    if (IndexBuffer.Buffer != VK_NULL_HANDLE)
      Graphics::Vulkan::Buffer::Destroy(IndexBuffer);

    for (auto pool : UniformBufferPool)
      Graphics::Vulkan::DescriptorPool::Destroy(pool);

    for (auto buffer : StagingUniformBuffer)
      Graphics::Vulkan::Buffer::Destroy(buffer);

    for (auto buffer : UniformBuffer)
      Graphics::Vulkan::Buffer::Destroy(buffer);
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

  std::vector<Graphics::Vertex> GetVertices()
  {
    return Vertices;
  }
  std::vector<uint32_t> GetIndices()
  {
    return Indices;
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