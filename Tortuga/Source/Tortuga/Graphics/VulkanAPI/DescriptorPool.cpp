#include "DescriptorPool.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
DescriptorPool::DescriptorPool(Device *device, uint32_t size)
{
    _device = device;

    std::vector<VkDescriptorPoolSize> poolSize(2);
    {
        poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize[0].descriptorCount = size;

        poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize[1].descriptorCount = size;
    }

    auto poolInfo = VkDescriptorPoolCreateInfo();
    {
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = poolSize.size();
        poolInfo.pPoolSizes = poolSize.data();
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
}; // namespace Vulkan
}; // namespace Graphics
}; // namespace Tortuga