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
    Graphics::Vulkan::Buffer::Buffer StagingVertexBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingTextureBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingNormalBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingVertexIndexBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingTextureIndexBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingNormalIndexBuffer;

    void CleanUp()
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