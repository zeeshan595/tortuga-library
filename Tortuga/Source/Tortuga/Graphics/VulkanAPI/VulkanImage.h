#ifndef _VULKAN_API_VULKAN_IMAGE
#define _VULKAN_API_VULKAN_IMAGE

#include "DataStructures.h"
#include "Device.h"
#include "CommandPool.h"
#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
VulkanImageData CreateVulkanImage(DeviceData device, uint32_t width, uint32_t height, VkFormat imageViewFormat);
void DestroyVulkanImage(VulkanImageData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif