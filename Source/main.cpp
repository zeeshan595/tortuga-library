#include "./Tortuga.hpp"

struct Pixel
{
  float r;
  float g;
  float b;
};

const uint32_t WINDOW_WIDTH = 1920;
const uint32_t WINDOW_HEIGHT = 1088;

using namespace Tortuga;

int main()
{
  auto vulkan = Graphics::Vulkan::Instance::Create();

  for (uint32_t i = 0; i < vulkan.Devices.size(); i++)
    Console::Info("{0}: {1}: {2}", Console::Arguments() << i << vulkan.Devices[i].Score << vulkan.Devices[i].Properties.deviceName);

  auto device = vulkan.Devices[0];
  auto window = Graphics::Vulkan::Window::Create(vulkan, "Hello World", WINDOW_WIDTH, WINDOW_HEIGHT);
  auto swapchain = Graphics::Vulkan::Swapchain::Create(device, window);
  auto renderer = Graphics::Renderer::Create(vulkan, device, WINDOW_WIDTH, WINDOW_HEIGHT);
  Graphics::Renderer::BindDescriptorSets(renderer);

  while (!window.SignalClose)
  {
    uint32_t swapchainIndex = Graphics::Vulkan::Swapchain::AquireNextImage(swapchain);
    Graphics::Vulkan::Window::PollEvents(window);
    Graphics::Renderer::Render(renderer);
    auto swapchainImage = Graphics::Vulkan::Swapchain::GetImage(swapchain, swapchainIndex);

    auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
    auto command = Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::PRIMARY);
    Graphics::Vulkan::Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::Vulkan::Command::TransferImageLayout(command, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Graphics::Vulkan::Command::BufferToImage(command, renderer.StagingBuffer, swapchainImage, {0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT});
    Graphics::Vulkan::Command::TransferImageLayout(command, swapchainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    Graphics::Vulkan::Command::End(command);
    Graphics::Vulkan::Command::Submit({command}, device.Queues.Compute[0]);
    vkQueueWaitIdle(device.Queues.Compute[0]);
    Graphics::Vulkan::Swapchain::PresentImage(swapchain, swapchainIndex, device.Queues.Compute[0]);
    vkQueueWaitIdle(device.Queues.Compute[0]);
  }

  Graphics::Renderer::Destroy(renderer);
  Graphics::Vulkan::Swapchain::Destroy(swapchain);
  Graphics::Vulkan::Window::Destroy(window);
  Graphics::Vulkan::Instance::Destroy(vulkan);
  return 0;
}