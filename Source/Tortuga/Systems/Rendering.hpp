#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include <future>
#include <thread>

#include "../Core/ECS/Entity.hpp"
#include "../Core/ECS/System.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/DisplaySurface.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Core/Engine.hpp"

#include "../Components/Mesh.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Camera.hpp"

namespace Tortuga
{
namespace Systems
{
class Rendering : public Core::ECS::System
{
private:
  //vulkan instance and surface
  Graphics::Vulkan::Instance::Instance VulkanInstance;
  Graphics::DisplaySurface::DisplaySurface DisplaySurface;

  //general command pools
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool ComputeCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;

  //descriptor layouts
  std::vector<Graphics::Vulkan::DescriptorLayout::DescriptorLayout> DescriptorLayouts;

  //rendering
  std::vector<Graphics::Vulkan::Shader::Shader> Shaders;
  Graphics::Vulkan::RenderPass::RenderPass RenderPass;
  Graphics::Vulkan::Pipeline::Pipeline Pipeline;
  Graphics::Vulkan::Command::Command RenderCommand;

  //sync
  std::vector<Graphics::Vulkan::Semaphore::Semaphore> TransferSemaphore;
  std::vector<Graphics::Vulkan::Semaphore::Semaphore> RenderSemaphore;
  std::vector<Graphics::Vulkan::Fence::Fence> RenderFence;

  struct MeshView : public Core::ECS::Component
  {
    bool IsStatic;
    uint32_t IndexCount;
    Graphics::Vulkan::Device::Device DeviceInUse;
    Graphics::Vulkan::Buffer::Buffer StagingVertexBuffer;
    Graphics::Vulkan::Buffer::Buffer VertexBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingIndexBuffer;
    Graphics::Vulkan::Buffer::Buffer IndexBuffer;
    Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;
    Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
    Graphics::Vulkan::Command::Command RenderCommand;
    Graphics::Vulkan::Command::Command TransferCommand;
    glm::mat4 Transform = glm::mat4(1.0f);

    void Setup(Graphics::Vulkan::Device::Device device);
    void OnDestroy();
  };

  void SetupSemaphores(uint32_t size);

public:
  Rendering();
  ~Rendering();

  void Update();
};
} // namespace Systems
} // namespace Tortuga

#endif