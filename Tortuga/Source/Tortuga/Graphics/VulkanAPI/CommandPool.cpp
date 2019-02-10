#include "CommandPool.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
CommandPool::CommandPool(Device *device, bool canReRecord)
{
    _device = device;

    //Setup command pool
    auto poolInfo = VkCommandPoolCreateInfo();
    {
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = _device->GetQueueFamilyIndices().graphicsFamily.value();
        if (canReRecord)
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
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
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga