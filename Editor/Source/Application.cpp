#include <Tortuga.h>

using namespace Tortuga;

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

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
  Pixel Pixels[WINDOW_WIDTH * WINDOW_HEIGHT];
};

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();

  // Setup window
  auto window = Graphics::CreateVulkanWindow("Test", WINDOW_WIDTH, WINDOW_HEIGHT);
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
  Graphics::SetVulkanBufferData<InputBuffer>(inputBuffer, {WINDOW_WIDTH, WINDOW_HEIGHT});

  auto commandPool = Graphics::CreateVulkanCommandPool(vulkan.Devices[0]);
  auto command = Graphics::CreateVulkanCommand(commandPool);

  // Record command for rendering
  Graphics::VulkanCommandBegin(command);
  Graphics::VulkanCommandBindPipeline(command, pipeline);
  Graphics::VulkanCommandDispatch(command, WINDOW_WIDTH, WINDOW_HEIGHT, 1);
  Graphics::VulkanCommandEnd(command);

  // Submit command to GPU
  Graphics::VulkanCommandSubmit({command}, Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

  // Wait for queue to be idle
  Graphics::DeviceQueueWaitForIdle(vulkan.Devices[0],
                                   Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

  // Get swapchain renderImage being used
  auto fence = Graphics::CreateVulkanFence(vulkan.Devices[0]);
  uint32_t imageIndex = 0;
  Graphics::SwapchainAquireImage(swapchain, &imageIndex, VK_NULL_HANDLE, fence);
  Graphics::FenceWait({fence});
  Graphics::DestroyVulkanFence(fence);

  auto renderImage = Graphics::CreateVulkanImage(vulkan.Devices[0], WINDOW_WIDTH, WINDOW_HEIGHT);

  // Record command (buffer -> renderImage)
  command = Graphics::CreateVulkanCommand(commandPool);
  Graphics::VulkanCommandBegin(command);
  Graphics::VulkanCommandImageLayoutTransfer(
      command, renderImage.Image, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  Graphics::VulkanCommandCopyBufferToImage(command, outputBuffer, renderImage.Image,
                                           {0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT});
  Graphics::VulkanCommandImageLayoutTransfer(
      command, renderImage.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  Graphics::VulkanCommandImageLayoutTransfer(
      command, swapchain.Images[imageIndex], VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  Graphics::VulkanCommandBlitImage(
      command, renderImage.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
      swapchain.Images[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      {0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT});
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

  while (true) {
    SDL_Event event;
    if (SDL_PollEvent(&event) != 0) {
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        break;
      }
    }
  }

  Graphics::DestroyVulkanImage(renderImage);
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
