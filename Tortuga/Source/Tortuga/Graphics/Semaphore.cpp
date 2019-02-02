#include "Semaphore.h"

namespace Tortuga
{
Semaphore::Semaphore(Device *device)
{
    this->_device = device;
    
    auto semaphoreInfo = VkSemaphoreCreateInfo();
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(_device->GetVirtualDevice(), &semaphoreInfo, nullptr, &_semaphore) != VK_SUCCESS)
    {
        Console::Error("Failed to create semaphore for command buffer!");
    }
}

Semaphore::~Semaphore()
{
    vkDestroySemaphore(_device->GetVirtualDevice(), _semaphore, nullptr);
}
}; // namespace Tortuga