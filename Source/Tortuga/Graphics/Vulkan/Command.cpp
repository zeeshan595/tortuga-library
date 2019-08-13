#include "./Command.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Command
{
Command Create(Device::Device device, CommandPool::CommandPool pool, Type type)
{
  Command data = {};
  data.Device = device.Device;

  VkCommandBufferAllocateInfo commandInfo = {};
  {
    commandInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandInfo.pNext = 0;
    commandInfo.commandPool = pool.CommandPool;
    if (type == PRIMARY)
      commandInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    else
      commandInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    commandInfo.commandBufferCount = 1;
  }
  ErrorCheck::Callback(vkAllocateCommandBuffers(data.Device, &commandInfo, &data.Command));

  return data;
}
std::vector<Command> Create(Device::Device device, CommandPool::CommandPool pool, Type type, uint32_t amount)
{
  VkCommandBufferAllocateInfo commandInfo = {};
  {
    commandInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandInfo.pNext = 0;
    commandInfo.commandPool = pool.CommandPool;
    if (type == PRIMARY)
      commandInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    else
      commandInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    commandInfo.commandBufferCount = amount;
  }
  std::vector<VkCommandBuffer> commands(amount);
  ErrorCheck::Callback(vkAllocateCommandBuffers(device.Device, &commandInfo, commands.data()));

  std::vector<Command> data(commands.size());
  for (uint32_t i = 0; i < data.size(); i++)
  {
    data[i].Device = device.Device;
    data[i].Command = commands[i];
  }

  return data;
}

void Begin(Command data, VkCommandBufferUsageFlags usage)
{
  VkCommandBufferBeginInfo beginInfo = {};
  {
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = usage;
    beginInfo.pInheritanceInfo = 0;
  }
  ErrorCheck::Callback(vkBeginCommandBuffer(data.Command, &beginInfo));
}
void End(Command data)
{
  ErrorCheck::Callback(vkEndCommandBuffer(data.Command));
}
void CopyBuffer(Command data, Buffer::Buffer source, Buffer::Buffer destination, uint32_t sourceOffset, uint32_t destinationOffset)
{
  if (source.Size > destination.Size)
  {
    Console::Warning("Failed to copy buffer because source is larger than the destination");
    return;
  }

  VkBufferCopy bufferCopy = {};
  {
    bufferCopy.srcOffset = sourceOffset;
    bufferCopy.dstOffset = destinationOffset;
    bufferCopy.size = source.Size;
  }
  vkCmdCopyBuffer(data.Command, source.Buffer, destination.Buffer, 1, &bufferCopy);
}
void BindPipeline(Command data, VkPipelineBindPoint BindPoint, Pipeline::Pipeline pipeline, std::vector<DescriptorSets::DescriptorSets> descriptorSets)
{
  std::vector<VkDescriptorSet> vulkanDescriptorSets(descriptorSets.size());
  for (uint32_t i = 0; i < vulkanDescriptorSets.size(); i++)
  {
    vulkanDescriptorSets[i] = descriptorSets[i].set;
  }

  vkCmdBindPipeline(data.Command, BindPoint, pipeline.Pipeline);
  vkCmdBindDescriptorSets(data.Command, BindPoint, pipeline.Layout, 0, vulkanDescriptorSets.size(), vulkanDescriptorSets.data(), 0, VK_NULL_HANDLE);
}
void Compute(Command data, uint32_t x, uint32_t y, uint32_t z)
{
  vkCmdDispatch(data.Command, x, y, z);
}
void Submit(std::vector<Command> data, VkQueue queue, std::vector<Semaphore::Semaphore> wait, std::vector<Semaphore::Semaphore> signal)
{
  if (data.size() <= 0)
    return;

  std::vector<VkSemaphore> waitSemaphores(wait.size());
  for (uint32_t i = 0; i < wait.size(); i++)
    waitSemaphores[i] = wait[i].Semaphore;

  std::vector<VkSemaphore> signalSemaphores(signal.size());
  for (uint32_t i = 0; i < signal.size(); i++)
    signalSemaphores[i] = signal[i].Semaphore;

  std::vector<VkCommandBuffer> cmdBuffers(data.size());
  for (uint32_t i = 0; i < cmdBuffers.size(); i++)
    cmdBuffers[i] = data[i].Command;

  VkSubmitInfo submitInfo = {};
  {
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = cmdBuffers.size();
    submitInfo.pCommandBuffers = cmdBuffers.data();
    submitInfo.waitSemaphoreCount = waitSemaphores.size();
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.signalSemaphoreCount = signalSemaphores.size();
    submitInfo.pSignalSemaphores = signalSemaphores.data();
  }
  ErrorCheck::Callback(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
}
void TransferImageLayout(Command data, Image::Image image, VkImageLayout oldLayout, VkImageLayout newLayout)
{
  auto barrier = VkImageMemoryBarrier();
  {
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image.Image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  }
  vkCmdPipelineBarrier(data.Command, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}
void BufferToImage(Command data, Buffer::Buffer buffer, Image::Image image, glm::vec2 offset, glm::vec2 size)
{
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
    regionInfo.imageOffset = {offset.x, offset.y, 0};
    regionInfo.imageExtent = {size.x, size.y, 1};
  }
  vkCmdCopyBufferToImage(data.Command, buffer.Buffer, image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &regionInfo);
}
void CopyImage(Command data, Image::Image source, Image::Image destination)
{
  if (source.Width != destination.Width || source.Height != destination.Height)
  {
    Console::Warning("Cannot copy images if they are not the same size, please use blit iamge instead");
    return;
  }

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
    copyInfo.extent = {source.Width, source.Height, 1};
  }
  vkCmdCopyImage(data.Command, source.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destination.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyInfo);
}
void BlitImage(Command data, Image::Image source, Image::Image destination, glm::vec2 size, glm::vec2 destinationOffset, glm::vec2 sourceOffset)
{
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
    blitInfo.srcOffsets[0] = {sourceOffset.x, sourceOffset.y, 0};
    blitInfo.srcOffsets[1] = {size.x, size.y, 1};
    blitInfo.dstSubresource = subResource;
    blitInfo.dstOffsets[0] = {destinationOffset.x, destinationOffset.y, 0};
    blitInfo.dstOffsets[1] = {size.x, size.y, 1};
  }
  vkCmdBlitImage(data.Command, source.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destination.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blitInfo, VK_FILTER_LINEAR);
}
} // namespace Command
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga