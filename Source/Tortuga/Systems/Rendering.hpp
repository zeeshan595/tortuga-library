#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include <future>
#include <thread>
#include <vulkan/vulkan.h>

#include "../Graphics/Vulkan/DescriptorLayout.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/Pipeline.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/Vulkan/Window.hpp"
#include "../Graphics/Vulkan/Semaphore.hpp"
#include "../Graphics/Vulkan/Fence.hpp"

#include "../Core/System.hpp"
#include "../Core/Screen.hpp"
#include "../Core/Entity.hpp"
#include "../Console.hpp"

#include "../Components/Mesh.hpp"
#include "../Components/Transform.hpp"

namespace Tortuga
{
namespace Systems
{
class Rendering : public Core::System
{
private:
  struct RenderInfo
  {
    uint32_t WindowWidth;
    uint32_t WindowHeight;
  };

  //general
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool ComputeCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;
  Graphics::Vulkan::Fence::Fence RenderingWaiter;

  //geometry
  Graphics::Vulkan::Shader::Shader GeometryShader;
  Graphics::Vulkan::Pipeline::Pipeline GeometryPipeline;
  Graphics::Vulkan::Semaphore::Semaphore GeometrySemaphore;

  //combine meshes into single buffer
  Graphics::Vulkan::Buffer::Buffer MeshCombineBuffer;
  Graphics::Vulkan::Command::Command MeshCombineCommand;
  Graphics::Vulkan::Semaphore::Semaphore MeshCombineSemaphore;

  //rendering
  VkExtent2D SwapchainExtent;
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout OutRenderingDescriptorLayout;
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout InRenderingDescriptorLayout;
  Graphics::Vulkan::DescriptorPool::DescriptorPool RenderingDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet InRenderingDescriptorSet;
  Graphics::Vulkan::DescriptorSet::DescriptorSet OutRenderingDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer RenderingBuffer;
  Graphics::Vulkan::Buffer::Buffer RenderingInfoBufferStaging;
  Graphics::Vulkan::Buffer::Buffer RenderingInfoBuffer;
  Graphics::Vulkan::Shader::Shader RenderingShader;
  Graphics::Vulkan::Pipeline::Pipeline RenderingPipeline;
  Graphics::Vulkan::Command::Command RenderingCommand;
  Graphics::Vulkan::Semaphore::Semaphore RenderingSemaphore;
  Graphics::Vulkan::Image::Image RenderingImage;

  //present
  Graphics::Vulkan::Command::Command PresentCommand;
  Graphics::Vulkan::Semaphore::Semaphore PresentSemaphore;
  static void UpdateWindowSize(Rendering *render);
  
public:
  void Update();
  void OnCreate();
  void OnDestroy();
};
} // namespace Systems
} // namespace Tortuga

#endif