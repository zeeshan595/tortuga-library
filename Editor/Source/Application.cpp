#include <Tortuga.h>

using namespace Tortuga;

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();
  auto window = Graphics::CreateWindow("Test", 800, 600);
  Graphics::CreateSurface(window, vulkan.Instance);
  auto swapchain = Graphics::CreateSwapchain(vulkan.Devices[0], window);
  auto buffer = Graphics::CreateVulkanBuffer(
      vulkan.Devices[0], sizeof(uint32_t), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  Graphics::UpdateVulkanBuffer(buffer, 25);

  Graphics::DestroyVulkanBuffer(buffer);
  Graphics::DestroySwapchain(swapchain);
  Graphics::DestroyWindow(window);
  Graphics::DestroyVulkanInstance(vulkan);

  SDL_Quit();
  return 0;
}