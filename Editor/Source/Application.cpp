#include <Tortuga.h>

using namespace Tortuga;

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();
  auto window = Graphics::CreateWindow("Test", 800, 600);
  Graphics::CreateSurface(window, vulkan.Instance);
  auto swapchain = Graphics::CreateSwapchain(vulkan.Devices[0], window);
  auto shader = Graphics::CreateVulkanShader(
      vulkan.Devices[0], Utils::GetFileContents("Build/Shaders/compute.spv"));
  auto pipeline = Graphics::CreatePipeline(
      vulkan.Devices[0], shader.ShaderModule,
      {{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT}});

  Graphics::DestroyVulkanShader(shader);
  Graphics::DestroyPipeline(pipeline);
  Graphics::DestroySwapchain(swapchain);
  Graphics::DestroyWindow(window);
  Graphics::DestroyVulkanInstance(vulkan);

  SDL_Quit();
  return 0;
}