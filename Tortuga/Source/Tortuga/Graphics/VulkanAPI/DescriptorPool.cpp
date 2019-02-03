#include "DescriptorPool.h"

namespace Tortuga
{
DescriptorPool::DescriptorPool(Device *device, uint32_t size)
{
    _device = device;

    auto poolSize = VkDescriptorPoolSize();
    {
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = size;
    }

    auto poolInfo = VkDescriptorPoolCreateInfo();
    {
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = size;
    }

    if (vkCreateDescriptorPool(device->GetVirtualDevice(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create descriptor pool!");
    }
}

DescriptorPool::~DescriptorPool()
{
    vkDestroyDescriptorPool(_device->GetVirtualDevice(), _descriptorPool, nullptr);
}
}; // namespace Tortuga