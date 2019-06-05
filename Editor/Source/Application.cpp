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

  auto pipeline =
      Graphics::CreateVulkanPipeline(vulkan.Devices[0], shader.ShaderModule, 2);

  auto tempBuffer1 =
      Graphics::CreateVulkanBuffer(vulkan.Devices[0], sizeof(uint32_t),
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  auto tempBuffer2 =
      Graphics::CreateVulkanBuffer(vulkan.Devices[0], sizeof(uint32_t),
                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

  Graphics::UpdateDescriptors(pipeline, {tempBuffer1, tempBuffer2});

  // Example
  auto commandPoolInfo = VkCommandPoolCreateInfo();
  {
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = 0;
    commandPoolInfo.flags = 0;
    commandPoolInfo.queueFamilyIndex =
        vulkan.Devices[0].QueueFamilies.ComputeFamily.value();
  }
  VkCommandPool commandPool;
  Graphics::ErrorCheck(vkCreateCommandPool(vulkan.Devices[0].VirtualDevice,
                                           &commandPoolInfo, nullptr,
                                           &commandPool));

  auto commandBufferAllocateInfo = VkCommandBufferAllocateInfo();
  {
    commandBufferAllocateInfo.sType =
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = 0;
    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 1;
  }
  VkCommandBuffer commandBuffer;
  Graphics::ErrorCheck(vkAllocateCommandBuffers(vulkan.Devices[0].VirtualDevice,
                                                &commandBufferAllocateInfo,
                                                &commandBuffer));

  auto commandBufferBeginInfo = VkCommandBufferBeginInfo();
  {
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    commandBufferBeginInfo.pInheritanceInfo = 0;
  }
  vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                    pipeline.Pipeline);

  Graphics::DestroyVulkanBuffer(tempBuffer1);
  Graphics::DestroyVulkanBuffer(tempBuffer2);
  Graphics::DestroyVulkanShader(shader);
  Graphics::DestroyVulkanPipeline(pipeline);
  Graphics::DestroyVulkanSwapchain(swapchain);
  Graphics::DestroyVulkanWindow(window);
  Graphics::DestroyVulkanInstance(vulkan);

  SDL_Quit();
  return 0;
}