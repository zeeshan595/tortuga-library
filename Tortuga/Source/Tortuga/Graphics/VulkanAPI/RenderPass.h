#ifndef _VULKAN_API_RENDER_PASS
#define _VULKAN_API_RENDER_PASS

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
RenderPassData CreateRenderPass(DeviceData device, VkFormat surfaceFormat);
void DestroyRenderPass(RenderPassData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif