#include "DescriptorSetLayout.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorSetLayout::DescriptorSetLayout(Device *device)
{
    this->_device = device;

    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    {
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    {
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;
    }

    if (vkCreateDescriptorSetLayout(_device->GetVirtualDevice(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create descriptor set layout!");
    }
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(_device->GetVirtualDevice(), _descriptorSetLayout, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga