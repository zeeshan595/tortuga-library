#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  const auto vulkan = Graphics::Vulkan::Instance::Create();
  const auto device = vulkan.Devices[0];

  auto window = Graphics::Vulkan::Window::Create(vulkan, "Tortuga", 800, 600);
  auto swapchain = Graphics::Vulkan::Swapchain::Create(device, window);

  const auto renderer = Graphics::Renderer::Create(vulkan, device, window.Width, window.Height, {});
  Graphics::Renderer::BindDescriptorSets(renderer);

  const auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  const auto command = Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::PRIMARY);

  while (!window.SignalClose)
  {
    Graphics::Vulkan::Window::PollEvents(window);
    Graphics::Renderer::Render(renderer);
    const auto swapchainIndex = Graphics::Vulkan::Swapchain::AquireNextImage(swapchain);
    const auto swapchainImage = Graphics::Vulkan::Swapchain::GetImage(swapchain, swapchainIndex);

    //Record Command
    {
      Graphics::Vulkan::Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
      Graphics::Vulkan::Command::TransferImageLayout(command, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      Graphics::Vulkan::Command::BlitImage(command, renderer.Image, swapchainImage, {renderer.Width, renderer.Height}, {0, 0}, {0, 0});
      Graphics::Vulkan::Command::TransferImageLayout(command, swapchainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
      Graphics::Vulkan::Command::End(command);
    }

    //Copy image to swapchain
    Graphics::Vulkan::Command::Submit({command}, device.Queues.Graphics[0]);
    Graphics::Vulkan::Device::WaitForQueue(device.Queues.Transfer[0]);
    Graphics::Vulkan::Swapchain::PresentImage(swapchain, swapchainIndex, device.Queues.Present);
  }

  Graphics::Vulkan::Device::WaitForDevice(device);
  Graphics::Vulkan::CommandPool::Destroy(commandPool);
  Graphics::Renderer::Destroy(renderer);
  Graphics::Vulkan::Swapchain::Destroy(swapchain);
  Graphics::Vulkan::Window::Destroy(window);
  Graphics::Vulkan::Instance::Destroy(vulkan);
}