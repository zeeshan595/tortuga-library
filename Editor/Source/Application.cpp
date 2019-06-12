#include <Tortuga.h>

using namespace Tortuga;

const uint32_t WINDOW_WIDTH = 1920;
const uint32_t WINDOW_HEIGHT = 1080;

int main(int argc, char **argv) {
  Core::CreateEngine();
  Core::CreateSystem<Systems::RenderingSystem>();
  Core::EngineMainLoop();
  Core::DestroyEngine();
  /*
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();

  // Setup window
  auto window =
      Graphics::CreateVulkanWindow("Test", WINDOW_WIDTH, WINDOW_HEIGHT);
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

  // Insert data into the input buffer
  Graphics::SetVulkanBufferData<InputBuffer>(inputBuffer,
                                             {400, 0});

  auto commandPool = Graphics::CreateVulkanCommandPool(vulkan.Devices[0]);
  auto renderCommand = Graphics::CreateVulkanCommand(commandPool);

  //=================
  //===GPU=COMMAND===
  //=================
  Graphics::VulkanCommandBegin(renderCommand,
                               VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
  Graphics::VulkanCommandBindPipeline(renderCommand, pipeline);
  Graphics::VulkanCommandDispatch(renderCommand, WINDOW_WIDTH, WINDOW_HEIGHT,
                                  1);
  Graphics::VulkanCommandEnd(renderCommand);
  //=================
  //===GPU=COMMAND===
  //=================

  auto presentCommand = Graphics::CreateVulkanCommand(commandPool);
  auto fence = Graphics::CreateVulkanFence(vulkan.Devices[0]);
  auto renderImage = Graphics::CreateVulkanImage(vulkan.Devices[0],
                                                 WINDOW_WIDTH, WINDOW_HEIGHT);

  while (true) {

    SDL_Event event;
    if (SDL_PollEvent(&event) != 0) {
      if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
        break;
      }
    }

    //==================
    //===RENDER START===
    //==================

    //Execute compute shader
    Graphics::VulkanCommandSubmit({renderCommand},
                                  Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

    // Wait for queue to be idle
    Graphics::DeviceQueueWaitForIdle(vulkan.Devices[0],
                                     Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

    // Ask swapchain for current image
    uint32_t imageIndex = 0;
    Graphics::FenceReset({fence});
    Graphics::SwapchainAquireImage(swapchain, &imageIndex, VK_NULL_HANDLE,
                                   fence);
    Graphics::FenceWait({fence});

    //Record command for presenting image
    Graphics::VulkanCommandBegin(presentCommand,
                                 VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    // Copy compute shader output to renderImage
    Graphics::VulkanCommandImageLayoutTransfer(
        presentCommand, renderImage.Image, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Graphics::VulkanCommandCopyBufferToImage(presentCommand, outputBuffer,
                                             renderImage.Image, {0, 0},
                                             {WINDOW_WIDTH, WINDOW_HEIGHT});
    Graphics::VulkanCommandImageLayoutTransfer(
        presentCommand, renderImage.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    // Copy renderImage to swapchain
    Graphics::VulkanCommandImageLayoutTransfer(
        presentCommand, swapchain.Images[imageIndex], VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Graphics::VulkanCommandBlitImage(
        presentCommand, renderImage.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        swapchain.Images[imageIndex], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        {0, 0}, {WINDOW_WIDTH, WINDOW_HEIGHT});
    Graphics::VulkanCommandImageLayoutTransfer(
        presentCommand, swapchain.Images[imageIndex],
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    Graphics::VulkanCommandEnd(presentCommand);

    // Submit command to GPU
    Graphics::VulkanCommandSubmit({presentCommand},
                                  Graphics::VULKAN_QUEUE_TYPE_PRESENT);

    // Wait for queue to be idle
    Graphics::DeviceQueueWaitForIdle(vulkan.Devices[0],
                                     Graphics::VULKAN_QUEUE_TYPE_PRESENT);

    // Present swapchain image inside window
    Graphics::SwapchainPresentImage(swapchain, imageIndex);
    Graphics::DeviceQueueWaitForIdle(vulkan.Devices[0],
                                     Graphics::VULKAN_QUEUE_TYPE_PRESENT);
    //==================
    //====RENDER END====
    //==================
  }

  Graphics::DestroyVulkanFence(fence);
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
  */
  return 0;
}
