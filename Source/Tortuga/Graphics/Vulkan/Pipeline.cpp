#include "./Pipeline.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Pipeline
{
std::vector<VkDescriptorSetLayout> GetVulkanDescriptorLayouts(std::vector<DescriptorLayout::DescriptorLayout> layouts)
{
  std::vector<VkDescriptorSetLayout> setLayouts(layouts.size());
  for (uint32_t i = 0; i < setLayouts.size(); i++)
    setLayouts[i] = layouts[i].Layouts;
  return setLayouts;
}

Pipeline CreateComputePipeline(Device::Device device, std::vector<DescriptorLayout::DescriptorLayout> layouts, Shader::Shader shader)
{
  Pipeline data = {};

  auto setLayouts = GetVulkanDescriptorLayouts(layouts);
  VkPipelineLayoutCreateInfo layoutInfo = {};
  {
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = setLayouts.size();
    layoutInfo.pSetLayouts = setLayouts.data();
  }
  ErrorCheck::Callback(vkCreatePipelineLayout(device.Device, &layoutInfo, nullptr, &data.Layout));

  VkComputePipelineCreateInfo pipelineInfo = {};
  {
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    {
      pipelineInfo.stage = {};
      pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
      pipelineInfo.stage.module = shader.Shader;
      pipelineInfo.stage.pName = "main";
    }
    pipelineInfo.layout = data.Layout;
    pipelineInfo.basePipelineIndex = 0;
    pipelineInfo.basePipelineHandle = 0;
  }
  ErrorCheck::Callback(vkMergePipelineCaches(device.Device, data.Cache, 1, &shader.TempCache));
  ErrorCheck::Callback(vkCreateComputePipelines(device.Device, data.Cache, 1, &pipelineInfo, nullptr, &data.Pipeline));
  return data;
}

Pipeline GraphicsPipelineHelper(Pipeline &data, Device::Device device, std::vector<VkPipelineCache> shaderCaches, std::vector<VkPipelineShaderStageCreateInfo> shaderStages)
{
  VkPipelineVertexInputStateCreateInfo vertexInputState = {};
  {
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexAttributeDescriptionCount = 0;
    vertexInputState.pVertexAttributeDescriptions = nullptr;
    vertexInputState.vertexBindingDescriptionCount = 0;
    vertexInputState.pVertexBindingDescriptions = nullptr;
  }

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
  {
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.primitiveRestartEnable = false;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  }

  VkPipelineTessellationStateCreateInfo tessellationState = {};
  {
    tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellationState.patchControlPoints = 0;
  }

  VkViewport viewport = {};
  {
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = 800;
    viewport.height = 600;
    viewport.maxDepth = 0.01f;
    viewport.maxDepth = 1000.0f;
  }

  VkRect2D scissors = {};
  {
    scissors.offset = {0, 0};
    scissors.extent = {800, 600};
  }

  VkPipelineViewportStateCreateInfo viewportState = {};
  {
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissors;
  }

  VkPipelineRasterizationStateCreateInfo rasterizationState = {};
  {
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.depthClampEnable = false;
    rasterizationState.rasterizerDiscardEnable = false;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationState.depthBiasEnable = false;
  }

  VkPipelineMultisampleStateCreateInfo multiSample = {};
  {
    multiSample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multiSample.rasterizationSamples = VK_SAMPLE_COUNT_2_BIT;
    multiSample.sampleShadingEnable = true;
    multiSample.minSampleShading = 0.2f;
  }

  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
  {
    colorBlendAttachment.blendEnable = true;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC1_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
  }

  VkPipelineColorBlendStateCreateInfo colorBlend = {};
  {
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.logicOpEnable = true;
    colorBlend.logicOp = VK_LOGIC_OP_COPY;
    colorBlend.attachmentCount = 1;
    colorBlend.pAttachments = &colorBlendAttachment;
    colorBlend.blendConstants[0] = 0.0f;
    colorBlend.blendConstants[1] = 0.0f;
    colorBlend.blendConstants[2] = 0.0f;
    colorBlend.blendConstants[3] = 0.0f;
  }

  VkPipelineDynamicStateCreateInfo dynamicState = {};
  {
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 0;
    dynamicState.pDynamicStates = nullptr;
  }

  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  {
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pTessellationState = &tessellationState;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizationState;
    pipelineInfo.pMultisampleState = &multiSample;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlend;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = data.Layout;
    pipelineInfo.renderPass = nullptr;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = nullptr;
    pipelineInfo.basePipelineIndex = 0;
  }
  ErrorCheck::Callback(vkMergePipelineCaches(device.Device, data.Cache, shaderCaches.size(), shaderCaches.data()));
  ErrorCheck::Callback(vkCreateGraphicsPipelines(device.Device, data.Cache, 1, &pipelineInfo, nullptr, &data.Pipeline));
  return data;
}

Pipeline CreateGraphicsPipeline(Device::Device device, std::vector<DescriptorLayout::DescriptorLayout> layouts, Shader::Shader vertex, Shader::Shader fragment)
{
  Pipeline data = {};

  auto setLayouts = GetVulkanDescriptorLayouts(layouts);
  VkPipelineLayoutCreateInfo layoutInfo = {};
  {
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = setLayouts.size();
    layoutInfo.pSetLayouts = setLayouts.data();
  }
  ErrorCheck::Callback(vkCreatePipelineLayout(device.Device, &layoutInfo, nullptr, &data.Layout));

  std::vector<VkPipelineCache> shaderCaches = {vertex.TempCache, fragment.TempCache};
  std::vector<VkPipelineShaderStageCreateInfo> shaderStages(2);
  {
    //vertex shader
    shaderStages[0] = {};
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].module = vertex.Shader;
    shaderStages[0].pName = "main";
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;

    shaderStages[1] = {};
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].module = fragment.Shader;
    shaderStages[1].pName = "main";
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  }

  return GraphicsPipelineHelper(data, device, shaderCaches, shaderStages);
}

Pipeline CreateGraphicsPipeline(Device::Device device, std::vector<DescriptorLayout::DescriptorLayout> layouts, Shader::Shader vertex, Shader::Shader geometry, Shader::Shader fragment)
{
  Pipeline data = {};

  auto setLayouts = GetVulkanDescriptorLayouts(layouts);
  VkPipelineLayoutCreateInfo layoutInfo = {};
  {
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = setLayouts.size();
    layoutInfo.pSetLayouts = setLayouts.data();
  }
  ErrorCheck::Callback(vkCreatePipelineLayout(device.Device, &layoutInfo, nullptr, &data.Layout));

  std::vector<VkPipelineCache> shaderCaches = {vertex.TempCache, geometry.TempCache, fragment.TempCache};
  std::vector<VkPipelineShaderStageCreateInfo> shaderStages(3);
  {
    //vertex shader
    shaderStages[0] = {};
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].module = vertex.Shader;
    shaderStages[0].pName = "main";
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;

    shaderStages[1] = {};
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].module = geometry.Shader;
    shaderStages[1].pName = "main";
    shaderStages[1].stage = VK_SHADER_STAGE_GEOMETRY_BIT;

    shaderStages[2] = {};
    shaderStages[2].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[2].module = fragment.Shader;
    shaderStages[2].pName = "main";
    shaderStages[2].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  }

  return GraphicsPipelineHelper(data, device, shaderCaches, shaderStages);
}

void DestroyPipeline(Pipeline data)
{
  vkDestroyPipeline(data.Device, data.Pipeline, nullptr);
  vkDestroyPipelineLayout(data.Device, data.Layout, nullptr);
}
} // namespace Pipeline
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga