#include <Tortuga.h>

using namespace Tortuga;

struct InputBuffer {
  uint32_t ResolutionX;
  uint32_t ResolutionY;
};
struct Pixel {
  float r;
  float g;
  float b;
  float a;
};
struct OutputBuffer {
  Pixel Pixels[480000];
};

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();

  // Setup window
  auto window = Graphics::CreateVulkanWindow("Test", 800, 600);
  Graphics::CreateVulkanSurface(window, vulkan.Instance);
  auto swapchain = Graphics::CreateVulkanSwapchain(vulkan.Devices[0], window);

  // Create shader module
  auto shader = Graphics::CreateVulkanShader(
      vulkan.Devices[0], Utils::GetFileContents("Shaders/compute.spv"));

  // Create pipeline and setup data structure (buffers)
  auto pipeline =
      Graphics::CreateVulkanPipeline(vulkan.Devices[0], shader.ShaderModule, 2);
  auto inputBuffer =
      Graphics::CreateVulkanBuffer(vulkan.Devices[0], sizeof(InputBuffer),
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  auto outputBuffer =
      Graphics::CreateVulkanBuffer(vulkan.Devices[0], sizeof(OutputBuffer),
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  Graphics::UpdatePipelineDescriptors(pipeline, {inputBuffer, outputBuffer});

  // Insert data into the buffers
  Graphics::SetVulkanBufferData<InputBuffer>(inputBuffer, {800, 600});

  auto commandPool = Graphics::CreateVulkanCommandPool(vulkan.Devices[0]);
  auto command = Graphics::CreateVulkanCommand(commandPool);

  // Record command for rendering
  Graphics::VulkanCommandBegin(command);
  Graphics::VulkanCommandBindPipeline(command, pipeline);
  Graphics::VulkanCommandDispatch(command, 1, 1, 1);
  Graphics::VulkanCommandEnd(command);

  // Submit command to GPU
  Graphics::VulkanCommandSubmit({command}, Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

  // Wait for queue to be idle
  Graphics::DeviceQueueWaitForIdle(vulkan.Devices[0],
                                   Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

  // Get swapchain image being used
  auto fence = Graphics::CreateVulkanFence(vulkan.Devices[0]);
  uint32_t imageIndex = 0;
  Graphics::SwapchainAquireImage(swapchain, &imageIndex, VK_NULL_HANDLE, fence);
  Graphics::FenceWait({fence});
  Graphics::DestroyVulkanFence(fence);

  auto image = Graphics::CreateVulkanImage(vulkan.Devices[0], 800, 600);

  // Record command (buffer -> image)
  command = Graphics::CreateVulkanCommand(commandPool);
  Graphics::VulkanCommandBegin(command);
  Graphics::VulkanCommandImageLayoutTransfer(
      command, image.Image, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  Graphics::VulkanCommandCopyBufferToImage(command, outputBuffer, image.Image,
                                           {0, 0}, {800, 600});
  Graphics::VulkanCommandImageLayoutTransfer(
      command, image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  Graphics::VulkanCommandImageLayoutTransfer(
      command, swapchain.Images[imageIndex], VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  Graphics::VulkanCommandBlitImage(
      command, image.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      swapchain.Images[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  Graphics::VulkanCommandImageLayoutTransfer(
      command, swapchain.Images[imageIndex],
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
  Graphics::VulkanCommandEnd(command);

  // Submit command to GPU
  Graphics::VulkanCommandSubmit({command}, Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

  // Wait for queue to be idle
  Graphics::DeviceQueueWaitForIdle(vulkan.Devices[0],
                                   Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

  Graphics::SwapchainPresentImage(swapchain, imageIndex);
  Graphics::DeviceQueueWaitForIdle(vulkan.Devices[0],
                                   Graphics::VULKAN_QUEUE_TYPE_PRESENT);

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
