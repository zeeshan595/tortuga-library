#include "Buffer.h"

namespace Tortuga
{
Buffer::Buffer(Device *device, uint32_t bufferSize, VkBufferUsageFlags flag)
{
    _device = device;
    _size = bufferSize;

    SetupStagingBuffer();
    SetupDeviceBuffer(flag);
}

Buffer::~Buffer()
{
    vkDestroyBuffer(_device->GetVirtualDevice(), _stagingBuffer, nullptr);
    vkFreeMemory(_device->GetVirtualDevice(), _stagingMemory, nullptr);
}

void Buffer::SetupDeviceBuffer(VkBufferUsageFlags flag)
{
    auto bufferInfo = VkBufferCreateInfo();
    {
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = _size;
        bufferInfo.usage = flag;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    if (vkCreateBuffer(_device->GetVirtualDevice(), &bufferInfo, nullptr, &_deviceBuffer) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(_device->GetVirtualDevice(), _deviceBuffer, &memoryRequirements);

    auto allocateInfo = VkMemoryAllocateInfo();
    {
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    }
    if (vkAllocateMemory(_device->GetVirtualDevice(), &allocateInfo, nullptr, &_deviceMemory) != VK_SUCCESS)
    {
        Console::Fatal("Failed to allocate memory for buffer!");
    }
    vkBindBufferMemory(_device->GetVirtualDevice(), _deviceBuffer, _deviceMemory, 0);
}

void Buffer::SetupStagingBuffer()
{
    auto bufferInfo = VkBufferCreateInfo();
    {
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = _size;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    if (vkCreateBuffer(_device->GetVirtualDevice(), &bufferInfo, nullptr, &_stagingBuffer) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(_device->GetVirtualDevice(), _stagingBuffer, &memoryRequirements);

    auto allocateInfo = VkMemoryAllocateInfo();
    {
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }
    if (vkAllocateMemory(_device->GetVirtualDevice(), &allocateInfo, nullptr, &_stagingMemory) != VK_SUCCESS)
    {
        Console::Fatal("Failed to allocate memory for buffer!");
    }
    vkBindBufferMemory(_device->GetVirtualDevice(), _stagingBuffer, _stagingMemory, 0);
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

void Buffer::CopyToDevice()
{
    auto allocInfo = VkCommandBufferAllocateInfo();
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = _device->GetCommandPool();
        allocInfo.commandBufferCount = 1;
    }
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(_device->GetVirtualDevice(), &allocInfo, &commandBuffer);

    auto beginInfo = VkCommandBufferBeginInfo();
    {
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    auto copyRegion = VkBufferCopy();
    {
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = _size;
    }
    vkCmdCopyBuffer(commandBuffer, _stagingBuffer, _deviceBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    auto submitInfo = VkSubmitInfo();
    {
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
    }
    vkQueueSubmit(_device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_device->GetGraphicsQueue());

    vkFreeCommandBuffers(_device->GetVirtualDevice(), _device->GetCommandPool(), 1, &commandBuffer);
}
}; // namespace Tortuga