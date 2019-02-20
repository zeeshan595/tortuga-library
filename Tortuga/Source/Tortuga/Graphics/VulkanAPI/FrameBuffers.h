#ifndef _VULKAN_API_FRAME_BUFFERS
#define _VULKAN_API_FRAME_BUFFERS

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
FrameBufferData CreateFrameBuffers(DeviceData device, RenderPassData renderPass, std::vector<VulkanImageData> images, uint32_t width, uint32_t height);
void DestroyFrameBuffers(FrameBufferData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif