#include "Buffer.h"

namespace Tortuga
{
Buffer::Buffer(Device *device, uint32_t bufferSize, BufferType type, StorageType storage)
{
    _bufferType = type;
    _storageType = storage;
    _device = device;
    _size = bufferSize;

    if (storage == StorageType::SyncToDevice)
    {
        _hostBuffer = CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, GetMemoryFlags(MemoryType::HostMemory));
        _deviceBuffer = CreateBuffer(GetBufferFlags(type, storage), GetMemoryFlags(MemoryType::DeviceMemory));
    }
    else if (storage == StorageType::SyncToDevice)
    {
        _deviceBuffer = CreateBuffer(GetBufferFlags(type, storage), GetMemoryFlags(MemoryType::DeviceMemory));
    }
    else
    {
        Console::Error("Unknown storage type, failed to create buffer!");
    }
}

Buffer::~Buffer()
{
    if (_storageType == StorageType::SyncToDevice)
    {
        vkFreeMemory(_device->GetVirtualDevice(), _hostBuffer.Location, nullptr);
        vkDestroyBuffer(_device->GetVirtualDevice(), _hostBuffer.Handler, nullptr);
        vkFreeMemory(_device->GetVirtualDevice(), _deviceBuffer.Location, nullptr);
        vkDestroyBuffer(_device->GetVirtualDevice(), _deviceBuffer.Handler, nullptr);
    }
    else if (_storageType == StorageType::OnlyOnDevice)
    {
        vkFreeMemory(_device->GetVirtualDevice(), _deviceBuffer.Location, nullptr);
        vkDestroyBuffer(_device->GetVirtualDevice(), _deviceBuffer.Handler, nullptr);
    }
}

Buffer::BufferMemoryObject Buffer::CreateBuffer(VkBufferUsageFlags bufferFlags, VkMemoryPropertyFlags memoryFlags)
{
    auto bmo = BufferMemoryObject();

    auto bufferInfo = VkBufferCreateInfo();
    {
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = _size;
        bufferInfo.usage = bufferFlags;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    if (vkCreateBuffer(_device->GetVirtualDevice(), &bufferInfo, nullptr, &bmo.Handler) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(_device->GetVirtualDevice(), bmo.Handler, &memoryRequirements);

    auto allocateInfo = VkMemoryAllocateInfo();
    {
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, memoryFlags);
    }
    if (vkAllocateMemory(_device->GetVirtualDevice(), &allocateInfo, nullptr, &bmo.Location) != VK_SUCCESS)
    {
        Console::Fatal("Failed to allocate memory for buffer!");
    }
    vkBindBufferMemory(_device->GetVirtualDevice(), bmo.Handler, bmo.Location, 0);
    return bmo;
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
    vkCmdCopyBuffer(commandBuffer, _hostBuffer.Handler, _deviceBuffer.Handler, 1, &copyRegion);
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

VkBufferUsageFlags Buffer::GetBufferFlags(BufferType type, StorageType storage)
{
    if (storage == StorageType::OnlyOnDevice)
    {
        switch (type)
        {
        case BufferType::Vertex:
            return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        case BufferType::Index:
            return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        case BufferType::Uniform:
            return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
    }
    else if (storage == StorageType::SyncToDevice)
    {
        switch (type)
        {
        case BufferType::Vertex:
            return VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        case BufferType::Index:
            return VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        case BufferType::Uniform:
            return VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
    }
    else
    {
        Console::Error("Invalid storage type, failed to create buffer");
    }

    return -1;
}
VkMemoryPropertyFlags Buffer::GetMemoryFlags(MemoryType type)
{
    switch (type)
    {
    case MemoryType::HostMemory:
        return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    case MemoryType::DeviceMemory:
        return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }

    return -1;
}
}; // namespace Tortuga