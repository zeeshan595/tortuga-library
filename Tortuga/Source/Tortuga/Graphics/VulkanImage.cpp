#include "./VulkanImage.h"

namespace Tortuga {
namespace Graphics {
uint32_t FindMemoryType(VulkanDevice device, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties);

VulkanImage CreateVulkanImage(VulkanDevice device, uint32_t width,
                              uint32_t height) {
  auto data = VulkanImage();
  data.VirtualDevice = device.VirtualDevice;

  auto imageInfo = VkImageCreateInfo();
  {
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = 0;
    imageInfo.flags = 0;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    imageInfo.extent = {width, height, 1};
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {
        device.QueueFamilies.ComputeFamily.value(),
        device.QueueFamilies.PresentFamily.value()};

    if (device.QueueFamilies.ComputeFamily !=
        device.QueueFamilies.PresentFamily) {
      imageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
      imageInfo.queueFamilyIndexCount = 2;
      imageInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
      imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
      imageInfo.queueFamilyIndexCount = 0;     // Optional
      imageInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  }

  ErrorCheck(
      vkCreateImage(device.VirtualDevice, &imageInfo, nullptr, &data.Image));

  auto memoryRequirements = VkMemoryRequirements();
  vkGetImageMemoryRequirements(data.VirtualDevice, data.Image,
                               &memoryRequirements);

  auto allocInfo = VkMemoryAllocateInfo();
  {
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = 0;
    allocInfo.memoryTypeIndex =
        FindMemoryType(device, memoryRequirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    allocInfo.allocationSize = memoryRequirements.size;
  }
  ErrorCheck(vkAllocateMemory(data.VirtualDevice, &allocInfo, nullptr,
                              &data.DeviceMemory));
  ErrorCheck(
      vkBindImageMemory(data.VirtualDevice, data.Image, data.DeviceMemory, 0));
  return data;
}
void DestroyVulkanImage(VulkanImage data) {
  vkDestroyImage(data.VirtualDevice, data.Image, nullptr);
  vkFreeMemory(data.VirtualDevice, data.DeviceMemory, nullptr);
}
} // namespace Graphics
} // namespace Tortuga