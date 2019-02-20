#include "VulkanImage.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
VulkanImageData CreateVulkanImage(DeviceData device, uint32_t width, uint32_t height, VkFormat imageViewFormat)
{
  auto data = VulkanImageData();
  data.Device = device.Device;

  auto imageInfo = VkImageCreateInfo();
  {
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imageInfo.format = imageViewFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.extent = {width, height, 1};
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.flags = 0;
  }

  if (vkCreateImage(data.Device, &imageInfo, nullptr, &data.Image))
  {
    Console::Fatal("Failed to create vulkan image on device: {0}", device.Properties.deviceName);
  }

  VkMemoryRequirements memoryRequirements;
  vkGetImageMemoryRequirements(data.Device, data.Image, &memoryRequirements);

  auto allocInfo = VkMemoryAllocateInfo();
  {
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(device, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  }

  if (vkAllocateMemory(data.Device, &allocInfo, nullptr, &data.Memory) != VK_SUCCESS)
  {
    Console::Fatal("Failed to allocate image memory on device: {0}", device.Properties.deviceName);
  }

  if (vkBindImageMemory(data.Device, data.Image, data.Memory, 0) != VK_SUCCESS)
  {
    Console::Fatal("Failed to bind image memory on device: {0}", device.Properties.deviceName);
  }

  auto imageViewInfo = VkImageViewCreateInfo();
  {
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.image = data.Image;
    imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewInfo.format = imageViewFormat;
    imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewInfo.subresourceRange.baseMipLevel = 0;
    imageViewInfo.subresourceRange.levelCount = 1;
    imageViewInfo.subresourceRange.baseArrayLayer = 0;
    imageViewInfo.subresourceRange.layerCount = 1;
  }

  if (vkCreateImageView(data.Device, &imageViewInfo, nullptr, &data.ImageView) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create image view on device: {0}", device.Properties.deviceName);
  }

  //Change image layout
  auto commandPool = CreateCommandPool(device);
  auto commandBuffer = CreateCommandBuffer(device, commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
  BeginCommandBufferRecording(commandBuffer, 0);
  CommandBufferImageLayoutTransfer(commandBuffer, 0, data.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  EndCommandBufferRecording(commandBuffer, 0);
  CommandBufferSubmit(commandBuffer, device.GraphicQueue, true);
  DestroyCommandPool(commandPool);

  return data;
}
void DestroyVulkanImage(VulkanImageData data)
{
  vkDestroyImageView(data.Device, data.ImageView, nullptr);
  vkDestroyImage(data.Device, data.Image, nullptr);
  vkFreeMemory(data.Device, data.Memory, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga