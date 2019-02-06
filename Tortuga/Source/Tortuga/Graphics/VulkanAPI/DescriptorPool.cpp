#include "DescriptorPool.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorPool::DescriptorPool(Device *device, std::vector<PoolType> types)
{
    _device = device;

    std::vector<VkDescriptorPoolSize> poolSize(types.size());
    for (uint32_t i = 0; i < poolSize.size(); i++)
    {
        if (types[i] == PoolType::Buffer)
        {
            poolSize[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSize[i].descriptorCount = 1;
        }
        else if (types[i] == PoolType::Image)
        {
            poolSize[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSize[i].descriptorCount = 1;
        }
    }

    auto poolInfo = VkDescriptorPoolCreateInfo();
    {
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = poolSize.size();
        poolInfo.pPoolSizes = poolSize.data();
        poolInfo.maxSets = 1;
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
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga