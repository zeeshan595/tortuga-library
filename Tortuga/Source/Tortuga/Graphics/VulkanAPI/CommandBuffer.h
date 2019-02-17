#ifndef _VULKAN_API_COMMAND_BUFFER
#define _VULKAN_API_COMMAND_BUFFER

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
void BeginCommandBufferRecording(CommandBufferData data, uint32_t index);
void BeginCommandBufferRecording(CommandBufferData data, uint32_t index, RenderPassData renderPass, uint32_t subPass);
void EndCommandBufferRecording(CommandBufferData data, uint32_t index);
CommandBufferData CreateCommandBuffer(DeviceData device, CommandPoolData pool, VkCommandBufferLevel level, uint32_t size = 1);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif