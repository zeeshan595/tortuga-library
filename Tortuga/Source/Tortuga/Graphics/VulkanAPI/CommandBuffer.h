#ifndef _VULKAN_API_COMMAND_BUFFER
#define _VULKAN_API_COMMAND_BUFFER

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
void CommandBufferBindDescriptorSet(CommandBufferData data, uint32_t index, VkPipelineLayout pipelineLayout, std::vector<VkDescriptorSet> descriptorSets);
void CommandBufferCopyBuffer(CommandBufferData data, uint32_t index, BufferData src, BufferData dst);
void CommandBufferSubmit(CommandBufferData data, VkQueue queue, bool waitUntilComplete = false);
void CommandBufferImageLayoutTransfer(CommandBufferData data, uint32_t index, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
void SetScissors(CommandBufferData data, uint32_t index, VkRect2D rect);
void SetViewport(CommandBufferData data, uint32_t index, VkViewport viewport);
void CommandBufferDraw(CommandBufferData data, uint32_t index, BufferData vertices, BufferData indices, uint16_t indexSize);
void BindCommandBufferPipeline(CommandBufferData data, uint32_t index, PipelineData pipeline);
void BeginCommandBufferRenderPass(CommandBufferData data, uint32_t index, VkFramebuffer framebuffer, VkRenderPass renderPass, VkOffset2D offset, VkExtent2D extent);
void EndCommandBufferRenderPass(CommandBufferData data, uint32_t index);
void BeginCommandBufferRecording(CommandBufferData data, uint32_t index);
void BeginCommandBufferRecording(CommandBufferData data, uint32_t index, RenderPassData renderPass, uint32_t subPass);
void EndCommandBufferRecording(CommandBufferData data, uint32_t index);
CommandBufferData CreateCommandBuffer(DeviceData device, CommandPoolData pool, VkCommandBufferLevel level, uint32_t size = 1);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif