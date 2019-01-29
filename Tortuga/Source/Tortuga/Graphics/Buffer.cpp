#include "Buffer.h"

namespace Tortuga
{
Buffer::Buffer(Device *device, uint32_t bufferSize)
{
    _device = device;
    _size = bufferSize;

    auto bufferInfo = VkBufferCreateInfo();
    {
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    if (vkCreateBuffer(_device->GetVirtualDevice(), &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(_device->GetVirtualDevice(), _buffer, &memoryRequirements);

    auto allocateInfo = VkMemoryAllocateInfo();
    {
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
    if (vkAllocateMemory(_device->GetVirtualDevice(), &allocateInfo, nullptr, &_bufferMemory) != VK_SUCCESS)
    {
        Console::Fatal("Failed to allocate memory for buffer!");
    }
    vkBindBufferMemory(_device->GetVirtualDevice(), _buffer, _bufferMemory, 0);
}

Buffer::~Buffer()
{
    vkDestroyBuffer(_device->GetVirtualDevice(), _buffer, nullptr);
    vkFreeMemory(_device->GetVirtualDevice(), _bufferMemory, nullptr);
}

uint32_t Buffer::FindMemoryType(uint32_t filter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(_device->GetPhysicalDevice(), &memoryProperties);
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if ((filter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    Console::Fatal("Failed to find GPUs memory type!");
}
}; // namespace Tortuga