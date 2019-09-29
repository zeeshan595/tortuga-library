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
  std::vector<Graphics::Vulkan::DescriptorLayout::DescriptorLayout> DescriptorLayouts;
  Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool ComputeCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;

  //renderer
  Graphics::Vulkan::Shader::Shader RenderShader;
  Graphics::Vulkan::Pipeline::Pipeline RenderPipeline;
  Graphics::Vulkan::Buffer::Buffer RenderOptionsBuffer;
  Graphics::Vulkan::Buffer::Buffer RenderingBuffer;
  Graphics::Vulkan::DescriptorPool::DescriptorPool RenderOptionsDescriptorPool;
  Graphics::Vulkan::DescriptorPool::DescriptorPool RenderDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet RenderOptionsDescriptorSet;
  Graphics::Vulkan::DescriptorSet::DescriptorSet RenderDescriptorSet;
  Graphics::Vulkan::Image::Image RenderedImage;
  Graphics::Vulkan::Command::Command RenderCommand;
  Graphics::Vulkan::Command::Command PresentCommand;

  //sync
  Graphics::Vulkan::Semaphore::Semaphore RenderSemaphore;
  Graphics::Vulkan::Semaphore::Semaphore PresentSemaphore;
  Graphics::Vulkan::Fence::Fence PresentFence;

public:
  void Update() override;
  Rendering();
  ~Rendering();
};
} // namespace Systems
} // namespace Tortuga

#endif