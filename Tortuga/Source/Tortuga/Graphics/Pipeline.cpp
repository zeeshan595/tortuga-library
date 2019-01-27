#include "Pipeline.h"

namespace Tortuga
{
Pipeline::Pipeline(Swapchain *swapchain, std::string vertexPath, std::string fragmentPath)
{
    this->_swapchain = swapchain;
    this->_device = swapchain->GetDevice();

    _vertexShader = CreateShader(_device->GetVirtualDevice(), ReadFile(vertexPath));
    _fragmentShader = CreateShader(_device->GetVirtualDevice(), ReadFile(fragmentPath));

    auto vertexCreateInfo = VkPipelineShaderStageCreateInfo();
    {
        vertexCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertexCreateInfo.module = _vertexShader;
        vertexCreateInfo.pName = "main";
    }
    auto fragmentCreateInfo = VkPipelineShaderStageCreateInfo();
    {
        fragmentCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentCreateInfo.module = _fragmentShader;
        fragmentCreateInfo.pName = "main";
    }

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {vertexCreateInfo, fragmentCreateInfo};

    //=================
    //====PIPE LINE====
    //=================

    //Vertex input state
    auto vertexInputInfo = VkPipelineVertexInputStateCreateInfo();
    {
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional
    }

    //Input assembly
    auto inputAssembly = VkPipelineInputAssemblyStateCreateInfo();
    {
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;
    }

    //Viewport
    auto viewport = VkViewport();
    {
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapchain->GetWidth();
        viewport.height = (float)swapchain->GetHeight();
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
    }

    //Scissor
    auto scissor = VkRect2D();
    {
        scissor.offset = {0, 0};
        scissor.extent = swapchain->GetExtent2D();
    }

    //Viewport state
    auto viewportState = VkPipelineViewportStateCreateInfo();
    {
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;
    }

    //Rasterizer
    auto rasterizer = VkPipelineRasterizationStateCreateInfo();
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

    //Multisampling
    auto multisampling = VkPipelineMultisampleStateCreateInfo();
    {
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;          // Optional
        multisampling.pSampleMask = nullptr;            // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE;      // Optional
    }

    //Color blend attachment
    auto colorBlendAttachment = VkPipelineColorBlendAttachmentState();
    {
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    }

    //Color blend state
    auto colorBlendState = VkPipelineColorBlendStateCreateInfo();
    {
        colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendState.logicOpEnable = VK_FALSE;
        colorBlendState.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlendState.attachmentCount = 1;
        colorBlendState.pAttachments = &colorBlendAttachment;
        colorBlendState.blendConstants[0] = 0.0f; // Optional
        colorBlendState.blendConstants[1] = 0.0f; // Optional
        colorBlendState.blendConstants[2] = 0.0f; // Optional
        colorBlendState.blendConstants[3] = 0.0f; // Optional
    }

    //Dynamic states
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH};
    auto dynamicState = VkPipelineDynamicStateCreateInfo();
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();

    //Pipeline layout
    auto pipelineLayoutInfo = VkPipelineLayoutCreateInfo();
    {
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;            // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr;         // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
    }
    if (vkCreatePipelineLayout(_device->GetVirtualDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
    {
        Console::Error("Failed to create pipeline!");
        return;
    }

    //Attachment description
    auto colorAttachment = VkAttachmentDescription();
    {
        colorAttachment.format = swapchain->GetSurfaceFormat().format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }

    auto colorAttachmentRef = VkAttachmentReference();
    {
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    auto subpass = VkSubpassDescription();
    {
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
    }

    //Render pass
    auto renderPassInfo = VkRenderPassCreateInfo();
    {
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
    }

    if (vkCreateRenderPass(_device->GetVirtualDevice(), &renderPassInfo, nullptr, &_renderPass) != VK_SUCCESS)
    {
        Console::Error("Failed to create render pass for vulkan pipeline");
        return;
    }

    //Vulkan Pipeline
    auto pipelineInfo = VkGraphicsPipelineCreateInfo();
    {
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlendState;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.layout = _pipelineLayout;
        pipelineInfo.renderPass = _renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional
    }
    if (vkCreateGraphicsPipelines(_device->GetVirtualDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS)
    {
        Console::Error("Failed to create graphics pipeline");
    }
}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(_device->GetVirtualDevice(), _graphicsPipeline, nullptr);
    vkDestroyRenderPass(_device->GetVirtualDevice(), _renderPass, nullptr);
    vkDestroyPipelineLayout(_device->GetVirtualDevice(), _pipelineLayout, nullptr);

    vkDestroyShaderModule(_device->GetVirtualDevice(), _vertexShader, nullptr);
    vkDestroyShaderModule(_device->GetVirtualDevice(), _fragmentShader, nullptr);
}

std::vector<char> Pipeline::ReadFile(const std::string &filename)
{
    std::ifstream file(filename.c_str(), std::ios::ate | std::ios::binary);
    if (!file.is_open())
        Console::Fatal("Failed to open file! {0}", filename.c_str());

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

VkShaderModule Pipeline::CreateShader(VkDevice device, std::vector<char> code)
{
    auto shaderCreateInfo = VkShaderModuleCreateInfo();
    {
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = code.size();
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
    }
    VkShaderModule shader;
    if (vkCreateShaderModule(device, &shaderCreateInfo, nullptr, &shader) != VK_SUCCESS)
    {
        Console::Error("Failed to load shader!");
    }
    return shader;
}
} // namespace Tortuga