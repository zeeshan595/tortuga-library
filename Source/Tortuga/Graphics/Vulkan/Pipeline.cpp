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
Pipeline CreateGraphicsPipeline(
    Device::Device device,
    Shader::Shader vertexShader,
    Shader::Shader fragmentShader,
    RenderPass::RenderPass renderPass,
    uint32_t width, uint32_t height,
    std::vector<VkVertexInputBindingDescription> bindings,
    std::vector<VkVertexInputAttributeDescription> attributes,
    std::vector<DescriptorLayout::DescriptorLayout> descriptorSetLayouts)
{
  Pipeline data = {};
  data.Device = device.Device;
  data.RenderPass = renderPass.RenderPass;

  VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
  {
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = bindings.size();
    vertexInputInfo.pVertexBindingDescriptions = bindings.data();
    vertexInputInfo.vertexAttributeDescriptionCount = attributes.size();
    vertexInputInfo.pVertexAttributeDescriptions = attributes.data();
  }

  VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
  {
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
  }

  VkViewport viewport = {};
  {
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
  }

  VkRect2D scissor = {};
  {
    scissor.offset = {0, 0};
    scissor.extent = {width, height};
  }

  VkPipelineViewportStateCreateInfo viewportState = {};
  {
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
  }

  VkPipelineRasterizationStateCreateInfo rasterizer = {};
  {
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f;          // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional
  }

  VkPipelineMultisampleStateCreateInfo multisampling = {};
  {
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;          // Optional
    multisampling.pSampleMask = nullptr;            // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE;      // Optional
  }

  VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
  {
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
  }

  VkPipelineDepthStencilStateCreateInfo depthStencil = {};
  {
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {};
    depthStencil.back = {};
  }

  VkPipelineColorBlendStateCreateInfo colorBlending = {};
  {
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional
  }

  std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};

  VkPipelineDynamicStateCreateInfo dynamicState = {};
  {
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();
  }

  auto descriptorLayouts = GetVulkanDescriptorLayouts(descriptorSetLayouts);
  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
  {
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = descriptorLayouts.size();
    pipelineLayoutInfo.pSetLayouts = descriptorLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
  }

  ErrorCheck::Callback(vkCreatePipelineLayout(data.Device, &pipelineLayoutInfo, nullptr, &data.Layout));

  std::vector<VkPipelineShaderStageCreateInfo> shaderModules(2);
  {
    shaderModules[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderModules[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderModules[0].module = vertexShader.Shader;
    shaderModules[0].pName = "main";

    shaderModules[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderModules[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderModules[1].module = fragmentShader.Shader;
    shaderModules[1].pName = "main";
  }

  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  {
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderModules.size();
    pipelineInfo.pStages = shaderModules.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = data.Layout;
    pipelineInfo.renderPass = data.RenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1;              // Optional
  }

  ErrorCheck::Callback(vkCreateGraphicsPipelines(data.Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &data.Pipeline));
  return data;
}
Pipeline CreateComputePipeline(Device::Device device, Shader::Shader shader, std::vector<char> cache, std::vector<DescriptorLayout::DescriptorLayout> layouts)
{
  Pipeline data = {};
  data.Device = device.Device;

  auto setLayouts = GetVulkanDescriptorLayouts(layouts);
  VkPipelineLayoutCreateInfo layoutInfo = {};
  {
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = setLayouts.size();
    layoutInfo.pSetLayouts = setLayouts.data();
  }
  ErrorCheck::Callback(vkCreatePipelineLayout(device.Device, &layoutInfo, nullptr, &data.Layout));

  VkPipelineCacheCreateInfo cacheInfo = {};
  {
    cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    cacheInfo.initialDataSize = cache.size();
    cacheInfo.pInitialData = cache.data();
  }
  ErrorCheck::Callback(vkCreatePipelineCache(device.Device, &cacheInfo, nullptr, &data.Cache));

  VkComputePipelineCreateInfo pipelineInfo = {};
  {
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    {
      pipelineInfo.stage = {};
      pipelineInfo.stage.sType =
          VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
      pipelineInfo.stage.module = shader.Shader;
      pipelineInfo.stage.pName = "main";
      pipelineInfo.stage.pSpecializationInfo = VK_NULL_HANDLE;
    }
    pipelineInfo.layout = data.Layout;
    pipelineInfo.basePipelineIndex = 0;
    pipelineInfo.basePipelineHandle = 0;
  }
  ErrorCheck::Callback(vkCreateComputePipelines(device.Device, data.Cache, 1, &pipelineInfo, nullptr, &data.Pipeline));
  return data;
}
void Destroy(Pipeline data)
{
  vkDestroyPipeline(data.Device, data.Pipeline, nullptr);
  vkDestroyPipelineLayout(data.Device, data.Layout, nullptr);
  vkDestroyPipelineCache(data.Device, data.Cache, nullptr);
}
std::vector<char> GetCacheData(Pipeline data)
{
  size_t cacheCount;
  ErrorCheck::Callback(vkGetPipelineCacheData(data.Device, data.Cache, &cacheCount, nullptr));
  std::vector<char> cacheData(cacheCount);
  ErrorCheck::Callback(vkGetPipelineCacheData(data.Device, data.Cache, &cacheCount, cacheData.data()));
  return cacheData;
}
} // namespace Pipeline
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga