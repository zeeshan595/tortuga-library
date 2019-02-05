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

    auto uboLayoutBinding = VkDescriptorSetLayoutBinding();
    {
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    }

    auto albedoTexture = VkDescriptorSetLayoutBinding();
    {
        albedoTexture.binding = 1;
        albedoTexture.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        albedoTexture.descriptorCount = 1;
        albedoTexture.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    std::vector<VkDescriptorSetLayoutBinding> bindings = {uboLayoutBinding, albedoTexture};
    auto layoutInfo = VkDescriptorSetLayoutCreateInfo();
    {
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = bindings.size();
        layoutInfo.pBindings = bindings.data();
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