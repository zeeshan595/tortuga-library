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
public:
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool RenderCommandPool;
  Graphics::Vulkan::Command::Command RenderCommand;
  Graphics::Vulkan::Command::Command TransferCommand;
  Graphics::Vulkan::Buffer::Buffer StagingVertexBuffer;
  Graphics::Vulkan::Buffer::Buffer VertexBuffer;
  Graphics::Vulkan::Buffer::Buffer StagingIndexBuffer;
  Graphics::Vulkan::Buffer::Buffer IndexBuffer;
  Graphics::Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
  std::vector<Graphics::Vulkan::DescriptorSet::DescriptorSet> DescriptorSets;
  Graphics::Vulkan::Buffer::Buffer StagingUniformBuffer;
  Graphics::Vulkan::Buffer::Buffer UniformBuffer;
  std::vector<Graphics::Vertex> Vertices;
  std::vector<uint32_t> Indices;

  Mesh()
  {
    TransferCommandPool.CommandPool = VK_NULL_HANDLE;
    RenderCommandPool.CommandPool = VK_NULL_HANDLE;
    TransferCommand.Command = VK_NULL_HANDLE;
    RenderCommand.Command = VK_NULL_HANDLE;
    StagingVertexBuffer.Buffer = VK_NULL_HANDLE;
    StagingIndexBuffer.Buffer = VK_NULL_HANDLE;
    DescriptorPool.Pool = VK_NULL_HANDLE;
    StagingUniformBuffer.Buffer = VK_NULL_HANDLE;
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

    if (DescriptorPool.Pool != VK_NULL_HANDLE)
      Graphics::Vulkan::DescriptorPool::Destroy(DescriptorPool);

    if (StagingUniformBuffer.Buffer != VK_NULL_HANDLE)
      Graphics::Vulkan::Buffer::Destroy(StagingUniformBuffer);

    if (UniformBuffer.Buffer != VK_NULL_HANDLE)
      Graphics::Vulkan::Buffer::Destroy(UniformBuffer);
  }
};
} // namespace Component
} // namespace Tortuga

#endif