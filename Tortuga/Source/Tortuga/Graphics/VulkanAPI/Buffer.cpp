#include "Buffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
Buffer::Buffer(Device *device, uint32_t bufferSize, BufferType bufferType, StorageType storageType)
{
    _bufferType = bufferType;
    _storageType = storageType;
    _device = device;
    _size = bufferSize;

    VkBufferUsageFlags bufferFlags;
    if (storageType == StorageType::DeviceCopy)
    {
        switch (bufferType)
        {
        case BufferType::Index:
            bufferFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
        case BufferType::Vertex:
            bufferFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            break;
        case BufferType::Uniform:
            bufferFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }

        //host buffer
        CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, _stagingBuffer, _stagingMemory);
        //device buffer
        CreateBuffer(bufferFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _deviceBuffer, _deviceMemory);
    }
    else if (storageType == StorageType::DeviceOnly)
    {
        switch (bufferType)
        {
        case BufferType::Index:
            bufferFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
        case BufferType::Vertex:
            bufferFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            break;
        case BufferType::Uniform:
            bufferFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }

        //device buffer
        CreateBuffer(bufferFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, _deviceBuffer, _deviceMemory);
    }
    else
    {
        Console::Fatal("Used unknown storage type for buffer!");
    }
}

Buffer::~Buffer()
{
    if (_storageType == StorageType::DeviceCopy)
    {        
        vkDestroyBuffer(_device->GetVirtualDevice(), _stagingBuffer, nullptr);
        vkFreeMemory(_device->GetVirtualDevice(), _stagingMemory, nullptr);
    }

    vkDestroyBuffer(_device->GetVirtualDevice(), _deviceBuffer, nullptr);
    vkFreeMemory(_device->GetVirtualDevice(), _deviceMemory, nullptr);
}

void Buffer::CreateBuffer(VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryProperties, VkBuffer &handler, VkDeviceMemory &deviceMemory)
{
    auto bufferInfo = VkBufferCreateInfo();
    {
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = _size;
        bufferInfo.usage = flags;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    if (vkCreateBuffer(_device->GetVirtualDevice(), &bufferInfo, nullptr, &handler) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(_device->GetVirtualDevice(), handler, &memoryRequirements);

    auto allocateInfo = VkMemoryAllocateInfo();
    {
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);
    }
    if (vkAllocateMemory(_device->GetVirtualDevice(), &allocateInfo, nullptr, &deviceMemory) != VK_SUCCESS)
    {
        Console::Fatal("Failed to allocate memory for buffer!");
    }
    vkBindBufferMemory(_device->GetVirtualDevice(), handler, deviceMemory, 0);
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
    return 0;
}

void Buffer::CopyToDevice(VkBuffer &hostBuffer, VkBuffer &deviceBuffer)
{
    CommandPool commandPool = CommandPool(_device);

    auto allocInfo = VkCommandBufferAllocateInfo();
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool.GetCommandPool();
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
    vkCmdCopyBuffer(commandBuffer, hostBuffer, deviceBuffer, 1, &copyRegion);
    vkEndCommandBuffer(commandBuffer);

    auto submitInfo = VkSubmitInfo();
    {
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
    }
    vkQueueSubmit(_device->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(_device->GetGraphicsQueue());

    vkFreeCommandBuffers(_device->GetVirtualDevice(), commandPool.GetCommandPool(), 1, &commandBuffer);
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga