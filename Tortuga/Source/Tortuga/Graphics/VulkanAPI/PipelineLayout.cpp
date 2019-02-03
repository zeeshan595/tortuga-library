#include "PipelineLayout.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
PipelineLayout::PipelineLayout(Device *device, std::vector<DescriptorSetLayout *> descriptorSetLayouts)
{
    this->_device = device;

    std::vector<VkDescriptorSetLayout> vulkanDescriptorSetLayouts(descriptorSetLayouts.size());
    for (uint32_t i = 0; i < vulkanDescriptorSetLayouts.size(); i++)
        vulkanDescriptorSetLayouts[i] = descriptorSetLayouts[i]->GetDescriptorSetLayout();

    auto pipelineLayoutInfo = VkPipelineLayoutCreateInfo();
    {
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = vulkanDescriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = vulkanDescriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional
    }
    if (vkCreatePipelineLayout(_device->GetVirtualDevice(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create pipeline!");
    }
}

PipelineLayout::~PipelineLayout()
{
    vkDestroyPipelineLayout(_device->GetVirtualDevice(), _pipelineLayout, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga