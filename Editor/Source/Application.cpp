#include <Tortuga.h>

using namespace Tortuga;

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();
  auto window = Graphics::CreateVulkanWindow("Test", 800, 600);
  Graphics::CreateVulkanSurface(window, vulkan.Instance);
  auto swapchain = Graphics::CreateVulkanSwapchain(vulkan.Devices[0], window);

  auto shader = Graphics::CreateVulkanShader(
      vulkan.Devices[0], Utils::GetFileContents("Shaders/compute.spv"));

  // Create pipeline and setup data structure (buffers)
  auto pipeline =
      Graphics::CreateVulkanPipeline(vulkan.Devices[0], shader.ShaderModule, 2);
  auto inputBuffer =
      Graphics::CreateVulkanBuffer(vulkan.Devices[0], sizeof(uint32_t),
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  auto outputBuffer =
      Graphics::CreateVulkanBuffer(vulkan.Devices[0], sizeof(uint32_t),
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  Graphics::UpdatePipelineDescriptors(pipeline, {inputBuffer, outputBuffer});

  // Insert data into the buffers
  Graphics::SetVulkanBufferData<uint32_t>(inputBuffer, 50);
  Graphics::SetVulkanBufferData<uint32_t>(outputBuffer, 0);

  auto commandPool = Graphics::CreateVulkanCommandPool(vulkan.Devices[0]);
  auto command = Graphics::CreateVulkanCommand(commandPool);

  Graphics::VulkanCommandBegin(command);
  Graphics::VulkanCommandBindPipeline(command, pipeline);
  Graphics::VulkanCommandDispatch(command, 1, 1, 1);
  Graphics::VulkanCommandEnd(command);

  Graphics::VulkanCommandSubmit({command}, vulkan.Devices[0].ComputeQueue);

  //Wait for queue to be idle
  vkQueueWaitIdle(vulkan.Devices[0].ComputeQueue);

  //Get data from compute shader
  auto data = Graphics::GetVulkanBufferData<uint32_t>(outputBuffer);
  Console::Info("Output from compute shader: {0}", data);

  Graphics::DestroyVulkanCommandPool(commandPool);
  Graphics::DestroyVulkanBuffer(inputBuffer);
  Graphics::DestroyVulkanBuffer(outputBuffer);
  Graphics::DestroyVulkanShader(shader);
  Graphics::DestroyVulkanPipeline(pipeline);
  Graphics::DestroyVulkanSwapchain(swapchain);
  Graphics::DestroyVulkanWindow(window);
  Graphics::DestroyVulkanInstance(vulkan);

  SDL_Quit();
  return 0;
}