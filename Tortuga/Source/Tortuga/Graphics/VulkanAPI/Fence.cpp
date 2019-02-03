#include "Fence.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
Fence::Fence(Device *device)
{
    this->_device = device;

    auto fenceInfo = VkFenceCreateInfo();
    {
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }
    if (vkCreateFence(_device->GetVirtualDevice(), &fenceInfo, nullptr, &_fence) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create fence for command buffer!");
    }
}

Fence::~Fence()
{
    vkDestroyFence(_device->GetVirtualDevice(), _fence, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga