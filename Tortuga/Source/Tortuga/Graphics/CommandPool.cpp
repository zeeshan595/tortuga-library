#include "CommandPool.h"

namespace Tortuga
{
CommandPool::CommandPool(Device *device)
{
    _device = device;

    //Setup command pool
    auto poolInfo = VkCommandPoolCreateInfo();
    {
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = _device->GetQueueFamilyIndices().graphicsFamily.value();
        poolInfo.flags = 0; // Optional
    }
    if (vkCreateCommandPool(_device->GetVirtualDevice(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create command pool for device!");
    }
}

CommandPool::~CommandPool()
{
    vkDestroyCommandPool(_device->GetVirtualDevice(), _commandPool, nullptr);
}
};