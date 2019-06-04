#include <Tortuga.h>

using namespace Tortuga;

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();
  auto window = Graphics::CreateWindow("Test", 800, 600);
  Graphics::CreateSurface(window, vulkan.Instance);
  auto swapchain = Graphics::CreateSwapchain(vulkan.Devices[0], window);
  auto buffer = Graphics::CreateVulkanBuffer(vulkan.Devices[0], 10);

  Graphics::DestroyVulkanBuffer(buffer);
  Graphics::DestroySwapchain(swapchain);
  Graphics::DestroyWindow(window);
  Graphics::DestroyVulkanInstance(vulkan);

  SDL_Quit();
  return 0;
}