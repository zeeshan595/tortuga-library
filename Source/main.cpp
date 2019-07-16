#include "./Tortuga.hpp"

using namespace Tortuga;

struct Test
{
  uint32_t x;
  uint32_t y;
  uint32_t z;
};

int main()
{
  auto vulkan = Graphics::Vulkan::Instance::Create();

  for (uint32_t i = 0; i < vulkan.Devices.size(); i++)
    Console::Info("{0}: {1}: {2}", Console::Arguments() << i << vulkan.Devices[i].Score << vulkan.Devices[i].Properties.deviceName);

  auto device = vulkan.Devices[0];
  auto window = Graphics::Vulkan::Window::Create(vulkan, "Hello World", 1024, 768);
  auto swapchain = Graphics::Vulkan::Swapchain::Create(device, window);
  auto image = Graphics::Vulkan::Image::Create(device, 800, 600);
  auto fence = Graphics::Vulkan::Fence::Create(device);
  auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
  Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::Type::PRIMARY);

  auto buffer = Graphics::Vulkan::Buffer::Create(device, sizeof(Test), VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  Test tempData = {5, 5, 5};
  Graphics::Vulkan::Buffer::SetData(buffer, &tempData, sizeof(Test));
  Graphics::Vulkan::Buffer::GetData(buffer, &tempData, sizeof(Test));
  Console::Info("{0},{1},{2}", Console::Arguments() << tempData.x << tempData.y << tempData.z);

  while (!window.SignalClose)
  {
    Graphics::Vulkan::Window::PollEvents(window);
  }

  Graphics::Vulkan::Buffer::Destroy(buffer);
  Graphics::Vulkan::CommandPool::Destroy(commandPool);
  Graphics::Vulkan::Fence::Destroy(fence);
  Graphics::Vulkan::Image::Destroy(image);
  Graphics::Vulkan::Swapchain::Destroy(swapchain);
  Graphics::Vulkan::Window::Destroy(window);
  Graphics::Vulkan::Instance::Destroy(vulkan);
  return 0;
}
