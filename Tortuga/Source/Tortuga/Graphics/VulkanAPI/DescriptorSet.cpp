#include "DescriptorSet.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{

DescriptorSet::DescriptorSet(Device *device, DescriptorSetLayout *descriptorSetLayout, DescriptorPool *descriptorPool, uint32_t size)
{
    this->_size = size;
    this->_device = device;
    this->_descriptorPool = descriptorPool;

    std::vector<VkDescriptorSetLayout> layouts(_size, descriptorSetLayout->GetDescriptorSetLayout());
    auto allocInfo = VkDescriptorSetAllocateInfo();
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool->GetDescriptorPool();
        allocInfo.descriptorSetCount = _size;
        allocInfo.pSetLayouts = layouts.data();
    }
    _descriptorSets.resize(_size);
    if (vkAllocateDescriptorSets(_device->GetVirtualDevice(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS)
    {
        Console::Fatal("Failed to allocate descriptor sets!");
    }
}

void DescriptorSet::UpdateDescriptorSet(std::vector<Buffer *> buffer, VulkanImage *image)
{
    for (uint32_t i = 0; i < _size; i++)
    {
        auto bufferInfo = VkDescriptorBufferInfo();
        {
            bufferInfo.buffer = buffer[i]->GetBuffer();
            bufferInfo.offset = 0;
            bufferInfo.range = buffer[i]->GetSize();
        }

        auto imageInfo = VkDescriptorImageInfo();
        {
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = image->GetImageView();
            imageInfo.sampler = image->GetSampler();
        }

        std::vector<VkWriteDescriptorSet> descriptorWrite(2);
        {
            descriptorWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite[0].dstSet = _descriptorSets[i];
            descriptorWrite[0].dstBinding = 0;
            descriptorWrite[0].dstArrayElement = 0;
            descriptorWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite[0].descriptorCount = 1;
            descriptorWrite[0].pBufferInfo = &bufferInfo;

            descriptorWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite[1].dstSet = _descriptorSets[i];
            descriptorWrite[1].dstBinding = 1;
            descriptorWrite[1].dstArrayElement = 0;
            descriptorWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrite[1].descriptorCount = 1;
            descriptorWrite[1].pImageInfo = &imageInfo;
        }
        vkUpdateDescriptorSets(_device->GetVirtualDevice(), descriptorWrite.size(), descriptorWrite.data(), 0, nullptr);
    }
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga