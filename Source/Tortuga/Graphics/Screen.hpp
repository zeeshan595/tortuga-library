#ifndef _RENDERER
#define _RENDERER

#include <future>
#include <thread>

#include "./Vulkan/Instance.hpp"
#include "./Vulkan/Window.hpp"
#include "./Vulkan/Swapchain.hpp"
#include "./Vulkan/DescriptorLayout.hpp"
#include "./Vulkan/DescriptorPool.hpp"
#include "./Vulkan/DescriptorSets.hpp"
#include "./Vulkan/Renderer.hpp"

#include "./Mesh.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Screen
{
struct FullWindow
{
  Vulkan::Device::Device Device;
  Vulkan::Window::Window Window;
  Vulkan::Swapchain::Swapchain Swapchain;
  Vulkan::DescriptorLayout::DescriptorLayout DescriptorLayout;
  Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
  Vulkan::Buffer::Buffer StagingBuffer;
  Vulkan::Buffer::Buffer Buffer;
  Vulkan::CommandPool::CommandPool CommandPool;
  Vulkan::CommandPool::CommandPool BufferTransferPool;
  Vulkan::Command::Command BufferTransferCommand;
  Vulkan::Renderer::Renderer Renderer;
  std::promise<void> *ExitSignal;
  std::thread *RenderThread;
};
struct MeshPreProcessor {
  Vulkan::Shader::Shader Shader;
  Vulkan::Pipeline::Pipeline Pipeline;
  Vulkan::DescriptorLayout::DescriptorLayout DescriptorLayout;
};
struct Screen
{
  uint32_t DeviceIndex;
  Vulkan::Instance::Instance Vulkan;
  std::vector<FullWindow> Windows;
  MeshPreProcessor MeshProcessor;

  Screen();
  ~Screen();
};
FullWindow CreateWindow(const char *title, uint32_t width, uint32_t height);
void DestroyWindow(FullWindow window);
void UpdateRenderData(FullWindow window, std::vector<Mesh> data);
SDL_Event PollEvents(FullWindow window);
void Initialize(uint32_t deviceIndex = 0);
Vulkan::Device::Device GetDevice();
Vulkan::Instance::Instance GetVulkan();
Vulkan::DescriptorLayout::DescriptorLayout GetMeshPreProcessorDescriptorLayout();
} // namespace Renderer
} // namespace Graphics
} // namespace Tortuga

#endif