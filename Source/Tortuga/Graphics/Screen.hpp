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
struct Screen
{
  Vulkan::Instance::Instance Vulkan;
  std::vector<FullWindow> Windows;

  Screen();
  ~Screen();
};
FullWindow CreateWindow(const char *title, uint32_t width, uint32_t height);
void DestroyWindow(FullWindow window);
void UpdateRenderData(FullWindow window, uint32_t data);
SDL_Event PollEvents(FullWindow window);
} // namespace Renderer
} // namespace Graphics
} // namespace Tortuga

#endif