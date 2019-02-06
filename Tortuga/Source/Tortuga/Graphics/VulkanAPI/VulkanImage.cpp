#include "VulkanImage.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
VulkanImage::VulkanImage(Device *device, uint32_t width, uint32_t height, uint32_t size, ImageType imageType)
{
    this->_width = width;
    this->_height = height;
    this->_size = size;
    this->_device = device;
    this->_imageType = imageType;

    VkImageUsageFlags flags;
    switch (imageType)
    {
    case ImageType::Color:
        flags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        break;
    case ImageType::Depth:
        break;
    }

    //Host
    CreateBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                 _stagingBuffer,
                 _stangingMemory);

    //Device
    CreateImage(width,
                height,
                VK_FORMAT_R8G8B8A8_UNORM,
                VK_IMAGE_TILING_OPTIMAL,
                flags,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                _deviceImage,
                _deviceImageMemory);

    auto viewInfo = VkImageViewCreateInfo();
    {
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = _deviceImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
    }
    if (vkCreateImageView(_device->GetVirtualDevice(), &viewInfo, nullptr, &_imageView) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create texture image view!");
    }

    auto samplerInfo = VkSamplerCreateInfo();
    {
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;
    }
    if (vkCreateSampler(_device->GetVirtualDevice(), &samplerInfo, nullptr, &_textureSampler) != VK_SUCCESS)
    {
        Console::Fatal("Failed to setup image sampler!");
    }
}

VulkanImage::~VulkanImage()
{
    vkDestroySampler(_device->GetVirtualDevice(), _textureSampler, nullptr);
    vkDestroyImageView(_device->GetVirtualDevice(), _imageView, nullptr);

    vkDestroyBuffer(_device->GetVirtualDevice(), _stagingBuffer, nullptr);
    vkFreeMemory(_device->GetVirtualDevice(), _stangingMemory, nullptr);

    vkDestroyImage(_device->GetVirtualDevice(), _deviceImage, nullptr);
    vkFreeMemory(_device->GetVirtualDevice(), _deviceImageMemory, nullptr);
}

void VulkanImage::CopyToDevice()
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

    TransitionImageLayout(commandBuffer, _deviceImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    //Copy staging buffer to image buffer
    auto copyRegion = VkBufferImageCopy();
    {
        copyRegion.bufferOffset = 0;
        copyRegion.bufferRowLength = 0;
        copyRegion.bufferImageHeight = 0;

        copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyRegion.imageSubresource.mipLevel = 0;
        copyRegion.imageSubresource.baseArrayLayer = 0;
        copyRegion.imageSubresource.layerCount = 1;

        copyRegion.imageOffset = {0, 0, 0};
        copyRegion.imageExtent = {
            static_cast<uint32_t>(_width),
            static_cast<uint32_t>(_height),
            1};
    }
    vkCmdCopyBufferToImage(commandBuffer, _stagingBuffer, _deviceImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

    TransitionImageLayout(commandBuffer, _deviceImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

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
}

void VulkanImage::TransitionImageLayout(VkCommandBuffer commandBuffer,
                                        VkImage image,
                                        VkFormat format,
                                        VkImageLayout oldLayout,
                                        VkImageLayout newLayout)
{
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    auto barrier = VkImageMemoryBarrier();
    {
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            Console::Fatal("Unsupported layout transition!");
        }
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier);
}

void VulkanImage::CreateBuffer(VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryProperties, VkBuffer &handler, VkDeviceMemory &deviceMemory)
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

void VulkanImage::CreateImage(uint32_t width,
                              uint32_t height,
                              VkFormat format,
                              VkImageTiling tiling,
                              VkImageUsageFlags usage,
                              VkMemoryPropertyFlags properties,
                              VkImage &image,
                              VkDeviceMemory &imageMemory)
{
    auto imageInfo = VkImageCreateInfo();
    {
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format; //VK_FORMAT_R8G8B8A8_UNORM;
        imageInfo.tiling = tiling; //VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage; //VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = 0; // Optional
    }
    if (vkCreateImage(_device->GetVirtualDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create vulkan image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(_device->GetVirtualDevice(), image, &memRequirements);

    auto allocInfo = VkMemoryAllocateInfo();
    {
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties); //VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    }
    if (vkAllocateMemory(_device->GetVirtualDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        Console::Fatal("Failed to allocate vulkan image memory!");
    }

    vkBindImageMemory(_device->GetVirtualDevice(), image, imageMemory, 0);
}

uint32_t VulkanImage::FindMemoryType(uint32_t filter, VkMemoryPropertyFlags properties)
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
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga