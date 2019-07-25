#include "./Tortuga.hpp"
#include <thread>
#include <future>

using namespace Tortuga;

int main()
{
  const auto vulkan = Graphics::Vulkan::Instance::Create();
  const auto device = vulkan.Devices[0];

  auto window = Graphics::Vulkan::Window::Create(vulkan, "Tortuga", 800, 600);
  auto swapchain = Graphics::Vulkan::Swapchain::Create(device, window);

  const auto renderer = Graphics::Renderer::Create(vulkan, device, window.Width, window.Height, {});
  Graphics::Renderer::BindDescriptorSets(renderer, {});

  const auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  const auto command = Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::PRIMARY);

  std::promise<void> exitSignal;
  std::thread renderThread(Graphics::Renderer::RenderThread, device, renderer, swapchain, command, std::move(exitSignal.get_future()));

  while (!window.SignalClose)
  {
    Graphics::Vulkan::Window::PollEvents(window);
  }

  exitSignal.set_value();
  Graphics::Vulkan::Device::WaitForDevice(device);
  Graphics::Vulkan::CommandPool::Destroy(commandPool);
  Graphics::Renderer::Destroy(renderer);
  Graphics::Vulkan::Swapchain::Destroy(swapchain);
  Graphics::Vulkan::Window::Destroy(window);
  Graphics::Vulkan::Instance::Destroy(vulkan);
}