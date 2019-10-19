#include "./Rendering.hpp"

namespace Tortuga
{
namespace Systems
{
Rendering::Rendering()
{
  //vulkan instance & display surface
  VulkanInstance = Graphics::Vulkan::Instance::Create();
  const auto device = VulkanInstance.Devices[0];
  DisplaySurface = Graphics::DisplaySurface::Create(VulkanInstance, VulkanInstance.Devices[0]);

  //command pools
  GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);

  //transfer swapchain images to present khr images
  {
    const auto command = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);
    Graphics::Vulkan::Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    for (const auto i : DisplaySurface.Swapchain.Images)
    {
      auto image = Graphics::Vulkan::Image::Image();
      image.Format = DisplaySurface.Swapchain.SurfaceFormat.format;
      image.Image = i;
      Graphics::Vulkan::Command::TransferImageLayout(command, image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    }
    Graphics::Vulkan::Command::End(command);
    Graphics::Vulkan::Command::Submit({command}, device.Queues.Graphics[0]);
    Graphics::Vulkan::Device::WaitForQueue(device.Queues.Graphics[0]);
  }
}
Rendering::~Rendering()
{
  Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  Graphics::DisplaySurface::Destroy(DisplaySurface);
  Graphics::Vulkan::Instance::Destroy(VulkanInstance);
}
void Rendering::Update()
{
  const auto device = VulkanInstance.Devices[0];
  const auto nextImage = Graphics::Vulkan::Swapchain::AquireNextImage(DisplaySurface.Swapchain);
  Graphics::Vulkan::Swapchain::PresentImage(DisplaySurface.Swapchain, nextImage, device.Queues.Present);
  Graphics::DisplaySurface::Dispatch(DisplaySurface);
}
} // namespace Systems
} // namespace Tortuga