#ifndef _VULKAN_PIPELINE
#define _VULKAN_PIPELINE

#include "./VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct VulkanPipelineBindingInfo {
  VkDescriptorType DescriptorType;
  VkShaderStageFlagBits ShaderStage;
};
struct VulkanPipeline {
  VkDevice VirtualDevice = VK_NULL_HANDLE;
  VkPipeline Pipeline = VK_NULL_HANDLE;
  VkPipelineLayout PipelineLayout = VK_NULL_HANDLE;
  VkDescriptorSetLayout PipelineSetsLayout = VK_NULL_HANDLE;
  VkPipelineCache PipelineCache = VK_NULL_HANDLE; // todo
};

VulkanPipeline CreatePipeline(VulkanDevice device, VkShaderModule shaderModule,
                              std::vector<VulkanPipelineBindingInfo> bindings);
void DestroyPipeline(VulkanPipeline pipeline);
} // namespace Graphics
} // namespace Tortuga

#endif