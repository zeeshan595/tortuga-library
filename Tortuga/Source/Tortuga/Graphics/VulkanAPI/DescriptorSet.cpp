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

void DescriptorSet::UpdateDescriptorSet(std::vector<Buffer *> buffer, uint32_t bufferObjectSize)
{
    if (buffer.size() != _size)
    {
        Console::Error("Descriptor size does not match buffers length!");
        return;
    }

    for (uint32_t i = 0; i < _size; i++)
    {
        auto bufferInfo = VkDescriptorBufferInfo();
        bufferInfo.buffer = buffer[i]->GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = bufferObjectSize;

        auto descriptorWrite = VkWriteDescriptorSet();
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = _descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;       // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional

        vkUpdateDescriptorSets(_device->GetVirtualDevice(), 1, &descriptorWrite, 0, nullptr);
    }
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga