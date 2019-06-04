#include "./VulkanPipeline.h"

namespace Tortuga {
namespace Graphics {
VulkanPipeline CreatePipeline(VulkanDevice device, VkShaderModule shaderModule,
                              std::vector<VulkanPipelineBindingInfo> bindings) {
  auto data = VulkanPipeline();
  data.VirtualDevice = device.VirtualDevice;

  std::vector<VkDescriptorSetLayoutBinding> setsBinding(bindings.size());
  for (uint32_t i = 0; i < bindings.size(); i++) {
    setsBinding[i] = VkDescriptorSetLayoutBinding();
    setsBinding[i].binding = i;
    setsBinding[i].descriptorType = bindings[i].DescriptorType;
    setsBinding[i].descriptorCount = 1;
    setsBinding[i].stageFlags = bindings[i].ShaderStage;
    setsBinding[i].pImmutableSamplers = 0;
  }

  auto setsInfo = VkDescriptorSetLayoutCreateInfo();
  {
    setsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    setsInfo.pNext = 0;
    setsInfo.flags = 0;
    setsInfo.bindingCount = setsBinding.size();
    setsInfo.pBindings = setsBinding.data();
  }
  ErrorCheck(vkCreateDescriptorSetLayout(device.VirtualDevice, &setsInfo,
                                         nullptr, &data.PipelineSetsLayout));

  auto layoutInfo = VkPipelineLayoutCreateInfo();
  {
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = 0;
    layoutInfo.flags = 0;
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &data.PipelineSetsLayout;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = 0;
  }
  ErrorCheck(vkCreatePipelineLayout(device.VirtualDevice, &layoutInfo, nullptr,
                                    &data.PipelineLayout));

  auto stageInfo = VkPipelineShaderStageCreateInfo();
  {
    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.pNext = 0;
    stageInfo.flags = 0;
    stageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    stageInfo.module = shaderModule;
    stageInfo.pName = "main";
    stageInfo.pSpecializationInfo = 0;
  }

  auto pipelineInfo = VkComputePipelineCreateInfo();
  {
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = 0;
    pipelineInfo.flags = 0;
    pipelineInfo.stage = stageInfo;
    pipelineInfo.layout = data.PipelineLayout;
    pipelineInfo.basePipelineHandle = 0;
    pipelineInfo.basePipelineIndex = 0;
  }
  ErrorCheck(vkCreateComputePipelines(device.VirtualDevice, 0, 1, &pipelineInfo,
                                      nullptr, &data.Pipeline));

  return data;
}
void DestroyPipeline(VulkanPipeline pipeline) {
  vkDestroyPipeline(pipeline.VirtualDevice, pipeline.Pipeline, nullptr);
  vkDestroyPipelineLayout(pipeline.VirtualDevice, pipeline.PipelineLayout,
                          nullptr);
  vkDestroyDescriptorSetLayout(pipeline.VirtualDevice,
                               pipeline.PipelineSetsLayout, nullptr);
}
} // namespace Graphics
} // namespace Tortuga