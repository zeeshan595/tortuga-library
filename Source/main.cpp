#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  auto vulkan = Graphics::Vulkan::Instance::Create();

  for (uint32_t i = 0; i < vulkan.Devices.size(); i++)
    Console::Info("{0}: {1}: {2}",
                  Console::Arguments() << i << vulkan.Devices[i].Score << vulkan.Devices[i].Properties.deviceName);

  auto device = vulkan.Devices[0];
  auto window = Graphics::Vulkan::Window::Create(vulkan, "Hello World", 1024, 768);
  auto swapchain = Graphics::Vulkan::Swapchain::Create(device, window);

  while (!window.SignalClose)
  {
    Graphics::Vulkan::Window::PollEvents(window);
  }

  Graphics::Vulkan::Swapchain::Destroy(swapchain);
  Graphics::Vulkan::Window::Destroy(window);
  Graphics::Vulkan::Instance::Destroy(vulkan);
  return 0;
}
