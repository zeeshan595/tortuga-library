#ifndef _VULKAN_PIPELINE
#define _VULKAN_PIPELINE

#include "./VulkanBuffer.h"
#include "./VulkanDevice.h"
#include "../Console.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct VulkanPipeline {
  VkDevice VirtualDevice = VK_NULL_HANDLE;
  VkPipeline Pipeline = VK_NULL_HANDLE;
  VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
  VkDescriptorSetLayout DescriptorSetsLayout = VK_NULL_HANDLE;
  VkDescriptorPool DescriptorPool;
  VkDescriptorSet DescriptorSets;
  uint32_t BindingSize;
  VkPipelineCache PipelineCache = VK_NULL_HANDLE; // todo
};

VulkanPipeline CreateVulkanPipeline(VulkanDevice device, VkShaderModule shaderModule,
                              uint32_t bindings);
void DestroyVulkanPipeline(VulkanPipeline pipeline);
void UpdateDescriptors(VulkanPipeline &pipeline,
                       std::vector<VulkanBuffer> buffers);
} // namespace Graphics
} // namespace Tortuga

#endif