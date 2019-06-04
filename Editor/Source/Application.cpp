#include <Tortuga.h>

using namespace Tortuga;

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();
  auto window = Graphics::CreateWindow("Test", 800, 600);
  Graphics::CreateSurface(window, vulkan.Instance);
  auto swapchain = Graphics::CreateSwapchain(vulkan.Devices[0], window);

  auto pipeline = Graphics::CreatePipeline(vulkan.Devices[0], VK_NULL_HANDLE, {
    {
      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
      VK_SHADER_STAGE_COMPUTE_BIT
    }
  });


  Graphics::DestroyPipeline(pipeline);
  Graphics::DestroySwapchain(swapchain);
  Graphics::DestroyWindow(window);
  Graphics::DestroyVulkanInstance(vulkan);

  SDL_Quit();
  return 0;
}