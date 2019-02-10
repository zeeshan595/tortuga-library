#include "DescriptorSetLayout.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorSetLayout::DescriptorSetLayout(Device *device, std::vector<DescriptorType> layouts)
{
    this->_layouts = layouts;
    this->_device = device;

    std::vector<VkDescriptorSetLayoutBinding> bindings(layouts.size());
    for (uint32_t i = 0; i < bindings.size(); i++)
    {
        bindings[i].binding = i;
        bindings[i].descriptorCount = 1;

        if (layouts[i].layout == LayoutType::Buffer)
            bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        else if (layouts[i].layout == LayoutType::Image)
            bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        if (layouts[i].shader == ShaderType::Vertex)
            bindings[i].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        else if (layouts[i].shader == ShaderType::Fragment)
            bindings[i].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

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