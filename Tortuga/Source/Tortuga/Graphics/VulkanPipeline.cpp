#include "./VulkanPipeline.h"

namespace Tortuga {
namespace Graphics {
VulkanPipeline CreateVulkanPipeline(VulkanDevice device, VkShaderModule shaderModule,
                              uint32_t bindingsSize) {
  auto data = VulkanPipeline();
  data.BindingSize = bindingsSize;
  data.VirtualDevice = device.VirtualDevice;

  std::vector<VkDescriptorSetLayoutBinding> setsBinding(bindingsSize);
  for (uint32_t i = 0; i < bindingsSize; i++) {
    setsBinding[i] = VkDescriptorSetLayoutBinding();
    setsBinding[i].binding = i;
    setsBinding[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    setsBinding[i].descriptorCount = 1;
    setsBinding[i].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
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
                                         nullptr, &data.DescriptorSetsLayout));

  auto layoutInfo = VkPipelineLayoutCreateInfo();
  {
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = 0;
    layoutInfo.flags = 0;
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &data.DescriptorSetsLayout;
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

  VkDescriptorPoolSize descriptorPoolSize = {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                                             bindingsSize};

  auto descriptorPoolInfo = VkDescriptorPoolCreateInfo();
  {
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.pNext = 0;
    descriptorPoolInfo.flags = 0;
    descriptorPoolInfo.maxSets = 1;
    descriptorPoolInfo.poolSizeCount = 1;
    descriptorPoolInfo.pPoolSizes = &descriptorPoolSize;
  }
  ErrorCheck(vkCreateDescriptorPool(device.VirtualDevice, &descriptorPoolInfo,
                                    nullptr, &data.DescriptorPool));

  auto descriptorInfo = VkDescriptorSetAllocateInfo();
  {
    descriptorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorInfo.pNext = 0;
    descriptorInfo.descriptorPool = data.DescriptorPool;
    descriptorInfo.descriptorSetCount = 1;
    descriptorInfo.pSetLayouts = &data.DescriptorSetsLayout;
  }
  ErrorCheck(vkAllocateDescriptorSets(device.VirtualDevice, &descriptorInfo,
                                      &data.DescriptorSets));

  return data;
}
void DestroyVulkanPipeline(VulkanPipeline pipeline) {
  vkDestroyPipeline(pipeline.VirtualDevice, pipeline.Pipeline, nullptr);
  vkDestroyPipelineLayout(pipeline.VirtualDevice, pipeline.PipelineLayout,
                          nullptr);
  vkDestroyDescriptorSetLayout(pipeline.VirtualDevice,
                               pipeline.DescriptorSetsLayout, nullptr);
  vkDestroyDescriptorPool(pipeline.VirtualDevice, pipeline.DescriptorPool,
                          nullptr);
}

void UpdateDescriptors(VulkanPipeline &pipeline,
                       std::vector<VulkanBuffer> buffers) {
  if (buffers.size() != pipeline.BindingSize) {
    Console::Error("Pipeline binding size must be equal to the number of "
                   "buffers provided");
    return;
  }

  std::vector<VkDescriptorBufferInfo> descriptorBufferInfos(buffers.size());
  std::vector<VkWriteDescriptorSet> writeDescriptorSets(buffers.size());
  for (uint32_t i = 0; i < buffers.size(); i++) {
    // Descriptor Set Buffer Info
    descriptorBufferInfos[i] = VkDescriptorBufferInfo();
    descriptorBufferInfos[i].buffer = buffers[i].Buffer,
    descriptorBufferInfos[i].offset = 0;
    descriptorBufferInfos[i].range = VK_WHOLE_SIZE;

    // Descriptor Set Write
    writeDescriptorSets[i] = VkWriteDescriptorSet();
    writeDescriptorSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[i].pNext = 0;
    writeDescriptorSets[i].dstSet = pipeline.DescriptorSets;
    writeDescriptorSets[i].dstBinding = i;
    writeDescriptorSets[i].dstArrayElement = 0;
    writeDescriptorSets[i].descriptorCount = 1;
    writeDescriptorSets[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writeDescriptorSets[i].pImageInfo = VK_NULL_HANDLE;
    writeDescriptorSets[i].pBufferInfo = &descriptorBufferInfos[i];
    writeDescriptorSets[i].pTexelBufferView = VK_NULL_HANDLE;
  }

  vkUpdateDescriptorSets(pipeline.VirtualDevice, buffers.size(),
                         writeDescriptorSets.data(), 0, 0);
}
} // namespace Graphics
} // namespace Tortuga