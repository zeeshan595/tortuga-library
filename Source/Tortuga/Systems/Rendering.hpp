#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include <cstdint>
#include <future>

#include "../Core/ECS/System.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/DisplaySurface.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/Shader.hpp"
#include "../Graphics/Vulkan/Pipeline.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Core/Engine.hpp"
#include "../Components/Mesh.hpp"

namespace Tortuga
{
namespace Systems
{
struct Rendering : public Core::ECS::System
{
private:
  struct RenderOptions
  {
    uint32_t renderWidth;
    uint32_t renderHeight;
  };
  Graphics::Vulkan::Instance::Instance VulkanInstance;
  Graphics::DisplaySurface::DisplaySurface DisplaySurface;
  Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool ComputeCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;

  //mesh
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout MeshDescriptorLayout;
  Graphics::Vulkan::DescriptorPool::DescriptorPool MeshDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet MeshDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer MeshVertexBuffer;
  Graphics::Vulkan::Buffer::Buffer MeshTextureBuffer;
  Graphics::Vulkan::Buffer::Buffer MeshNormalBuffer;
  Graphics::Vulkan::Buffer::Buffer MeshVertexIndexBuffer;
  Graphics::Vulkan::Buffer::Buffer MeshTextureIndexBuffer;
  Graphics::Vulkan::Buffer::Buffer MeshNormalIndexBuffer;
  Graphics::Vulkan::Command::Command MeshCopyCommand;

  //renderer
  Graphics::Vulkan::Shader::Shader RenderShader;
  Graphics::Vulkan::Pipeline::Pipeline RenderPipeline;
  Graphics::Vulkan::Buffer::Buffer RenderOptionsBuffer;
  Graphics::Vulkan::Buffer::Buffer RenderingBuffer;
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout RenderingDescriptorLayout;
  Graphics::Vulkan::DescriptorPool::DescriptorPool RenderDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet RenderDescriptorSet;
  Graphics::Vulkan::Image::Image RenderedImage;
  Graphics::Vulkan::Command::Command RenderCommand;
  Graphics::Vulkan::Command::Command PresentCommand;

  //sync
  Graphics::Vulkan::Semaphore::Semaphore MeshSemaphore;
  Graphics::Vulkan::Semaphore::Semaphore RenderSemaphore;
  Graphics::Vulkan::Semaphore::Semaphore PresentSemaphore;
  Graphics::Vulkan::Fence::Fence PresentFence;

  void SetupLayoutInformation(Graphics::Vulkan::Device::Device device);
  void ProcessMeshBuffers(Graphics::Vulkan::Device::Device device);

public:
  struct MeshView : public Core::ECS::Component
  {
    Graphics::Vulkan::Device::Device DeviceInUse;
    Graphics::Vulkan::Buffer::Buffer StagingVertexBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingTextureBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingNormalBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingVertexIndexBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingTextureIndexBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingNormalIndexBuffer;

    void OnCreate() override
    {
      const auto device = DeviceInUse;
      const auto mesh = Core::Engine::GetComponent<Components::Mesh>(Root);
      //vertex
      StagingVertexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->Vertices.size() * sizeof(glm::vec4), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      StagingVertexIndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->VertexIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      //texture
      StagingTextureBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->Textures.size() * sizeof(glm::vec2), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      StagingTextureIndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->TextureIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      //normals
      StagingNormalBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->Normals.size() * sizeof(glm::vec4), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      StagingNormalIndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->NormalIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

      //copy data to staging buffers
      Graphics::Vulkan::Buffer::SetData(StagingVertexBuffer, mesh->Vertices.data(), StagingVertexBuffer.Size);
      Graphics::Vulkan::Buffer::SetData(StagingTextureBuffer, mesh->Textures.data(), StagingTextureBuffer.Size);
      Graphics::Vulkan::Buffer::SetData(StagingNormalBuffer, mesh->Normals.data(), StagingNormalBuffer.Size);
      Graphics::Vulkan::Buffer::SetData(StagingVertexIndexBuffer, mesh->VertexIndices.data(), StagingVertexIndexBuffer.Size);
      Graphics::Vulkan::Buffer::SetData(StagingTextureIndexBuffer, mesh->TextureIndices.data(), StagingTextureIndexBuffer.Size);
      Graphics::Vulkan::Buffer::SetData(StagingNormalIndexBuffer, mesh->NormalIndices.data(), StagingNormalIndexBuffer.Size);
    }

    void OnDestroy() override
    {
      Graphics::Vulkan::Buffer::Destroy(StagingVertexBuffer);
      Graphics::Vulkan::Buffer::Destroy(StagingVertexIndexBuffer);
      Graphics::Vulkan::Buffer::Destroy(StagingTextureBuffer);
      Graphics::Vulkan::Buffer::Destroy(StagingTextureIndexBuffer);
      Graphics::Vulkan::Buffer::Destroy(StagingNormalBuffer);
      Graphics::Vulkan::Buffer::Destroy(StagingNormalIndexBuffer);
    }
  };

  void Update() override;
  Rendering();
  ~Rendering();
};
} // namespace Systems
} // namespace Tortuga

#endif