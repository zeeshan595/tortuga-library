#include "./VulkanCommand.h"

namespace Tortuga {
namespace Graphics {
VulkanCommandPool CreateVulkanCommandPool(VulkanDevice device) {
  auto data = VulkanCommandPool();
  data.Device = device;

  auto commandPoolInfo = VkCommandPoolCreateInfo();
  {
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = 0;
    commandPoolInfo.flags = 0;
    commandPoolInfo.queueFamilyIndex =
        device.QueueFamilies.ComputeFamily.value();
  }
  Graphics::ErrorCheck(vkCreateCommandPool(
      device.VirtualDevice, &commandPoolInfo, nullptr, &data.CommandPool));

  return data;
}
void DestroyVulkanCommandPool(VulkanCommandPool data) {
  vkDestroyCommandPool(data.Device.VirtualDevice, data.CommandPool, nullptr);
}
VulkanCommand CreateVulkanCommand(VulkanCommandPool commandPool) {
  auto data = VulkanCommand();
  data.CommandPool = commandPool;

  auto commandBufferAllocateInfo = VkCommandBufferAllocateInfo();
  {
    commandBufferAllocateInfo.sType =
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = 0;
    commandBufferAllocateInfo.commandPool = data.CommandPool.CommandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 1;
  }
  Graphics::ErrorCheck(vkAllocateCommandBuffers(
      data.CommandPool.Device.VirtualDevice, &commandBufferAllocateInfo,
      &data.CommandBuffer));

  return data;
}
void VulkanCommandBegin(VulkanCommand command) {
  auto commandBufferBeginInfo = VkCommandBufferBeginInfo();
  {
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    commandBufferBeginInfo.pInheritanceInfo = 0;
  }
  ErrorCheck(
      vkBeginCommandBuffer(command.CommandBuffer, &commandBufferBeginInfo));
}
void VulkanCommandEnd(VulkanCommand command) {
  ErrorCheck(vkEndCommandBuffer(command.CommandBuffer));
}
void VulkanCommandBindPipeline(VulkanCommand command, VulkanPipeline pipeline) {
  vkCmdBindPipeline(command.CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                    pipeline.Pipeline);
  vkCmdBindDescriptorSets(command.CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                          pipeline.PipelineLayout, 0, 1,
                          &pipeline.DescriptorSets, 0, 0);
}
void VulkanCommandDispatch(VulkanCommand command, uint32_t groupCountX,
                           uint32_t groupCountY, uint32_t groupCountZ) {
  vkCmdDispatch(command.CommandBuffer, groupCountX, groupCountY, groupCountZ);
}
void VulkanCommandSubmit(std::vector<VulkanCommand> command,
                         VulkanQueueType queueType) {
  if (command.size() == 0) {
    Console::Error("You must provide atleast 1 command to submit");
    return;
  }
  std::vector<VkCommandBuffer> cmdBuffers(command.size());
  for (uint32_t i = 0; i < command.size(); i++) {
    cmdBuffers[i] = command[i].CommandBuffer;
  }

  auto submitInfo = VkSubmitInfo();
  {
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = 0;
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = cmdBuffers.size();
    submitInfo.pCommandBuffers = cmdBuffers.data();
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;
  }
  switch (queueType) {
  case VULKAN_QUEUE_TYPE_COMPUTE:
    ErrorCheck(vkQueueSubmit(command[0].CommandPool.Device.ComputeQueue, 1,
                             &submitInfo, VK_NULL_HANDLE));
    break;
  case VULKAN_QUEUE_TYPE_PRESENT:
    ErrorCheck(vkQueueSubmit(command[0].CommandPool.Device.PresentQueue, 1,
                             &submitInfo, VK_NULL_HANDLE));
    break;
  default:
    Console::Error("This type of queue type is not supported");
    break;
  }
}
void VulkanCommandCopyBufferToImage(VulkanCommand command, VulkanBuffer buffer,
                                    VkImage image, glm::vec2 imageOffset,
                                    glm::vec2 imageSize) {
  auto imageLayers = VkImageSubresourceLayers();
  {
    imageLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageLayers.mipLevel = 0;
    imageLayers.baseArrayLayer = 0;
    imageLayers.layerCount = 1;
  }
  auto regionInfo = VkBufferImageCopy();
  {
    regionInfo.bufferOffset = 0;
    regionInfo.bufferRowLength = 0;
    regionInfo.bufferImageHeight = 0;
    regionInfo.imageSubresource = imageLayers;
    regionInfo.imageOffset = {imageOffset.x, imageOffset.y, 0};
    regionInfo.imageExtent = {imageSize.x, imageSize.y, 1};
  }
  vkCmdCopyBufferToImage(command.CommandBuffer, buffer.Buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &regionInfo);
}
void VulkanCommandImageLayoutTransfer(VulkanCommand command, VkImage image,
                                      VkImageLayout oldLayout,
                                      VkImageLayout newLayout) {
  auto barrier = VkImageMemoryBarrier();
  {
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  }

  vkCmdPipelineBarrier(command.CommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                       VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                       nullptr, 1, &barrier);
}
void VulkanCommandCopyImage(VulkanCommand command, VkImage source,
                            VkImageLayout sourceLayout, VkImage destination,
                            VkImageLayout destinationLayout) {
  auto subResource = VkImageSubresourceLayers();
  {
    subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subResource.mipLevel = 0;
    subResource.baseArrayLayer = 0;
    subResource.layerCount = 1;
  }
  auto copyInfo = VkImageCopy();
  {
    copyInfo.srcSubresource = subResource;
    copyInfo.srcOffset = {0, 0, 0};
    copyInfo.dstSubresource = subResource;
    copyInfo.dstOffset = {0, 0, 0};
    copyInfo.extent = {800, 600, 0};
  }
  vkCmdCopyImage(command.CommandBuffer, source, sourceLayout, destination,
                 destinationLayout, 1, &copyInfo);
}
void VulkanCommandBlitImage(VulkanCommand command, VkImage source,
                            VkImageLayout sourceLayout, VkImage destination,
                            VkImageLayout destinationLayout) {
  auto subResource = VkImageSubresourceLayers();
  {
    subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subResource.mipLevel = 0;
    subResource.baseArrayLayer = 0;
    subResource.layerCount = 1;
  }

  auto blitInfo = VkImageBlit();
  {
    blitInfo.srcSubresource = subResource;
    blitInfo.srcOffsets[0] = {0, 0, 0};
    blitInfo.srcOffsets[1] = {800, 600, 1};
    blitInfo.dstSubresource = subResource;
    blitInfo.dstOffsets[0] = {0, 0, 0};
    blitInfo.dstOffsets[1] = {800, 600, 1};
  }
  vkCmdBlitImage(command.CommandBuffer, source, sourceLayout, destination,
                 destinationLayout, 1, &blitInfo, VK_FILTER_LINEAR);
}
} // namespace Graphics
} // namespace Tortuga