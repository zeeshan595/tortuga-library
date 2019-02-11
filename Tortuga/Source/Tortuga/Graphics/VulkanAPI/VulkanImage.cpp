#include "VulkanImage.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
VulkanImage::VulkanImage(Device *device, uint32_t width, uint32_t height, ImageType imageType, bool sampledImage)
{
    this->_width = width;
    this->_height = height;
    this->_device = device;
    this->_imageType = imageType;

    if (_imageType == ImageType::Color)
    {
        _imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
        CreateImage(width,
                    height,
                    _imageFormat,
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    _deviceImage,
                    _deviceImageMemory);

        auto viewInfo = VkImageViewCreateInfo();
        {
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = _deviceImage;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = _imageFormat;
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
    }
    else if (_imageType == ImageType::Depth)
    {
        _imageFormat = FindDepthFormat(_device);
        CreateImage(width,
                    height,
                    _imageFormat,
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    _deviceImage,
                    _deviceImageMemory);

        auto viewInfo = VkImageViewCreateInfo();
        {
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = _deviceImage;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = _imageFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;
        }
        if (vkCreateImageView(_device->GetVirtualDevice(), &viewInfo, nullptr, &_imageView) != VK_SUCCESS)
        {
            Console::Fatal("Failed to create texture image view!");
        }
        auto commandPool = CommandPool(_device);
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
        TransitionImageLayout(commandBuffer, _deviceImage, _imageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
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

    if (sampledImage)
    {
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
    else
    {
        _textureSampler = VK_NULL_HANDLE;
    }
}

VulkanImage::~VulkanImage()
{
    if (_textureSampler != VK_NULL_HANDLE)
        vkDestroySampler(_device->GetVirtualDevice(), _textureSampler, nullptr);

    vkDestroyImageView(_device->GetVirtualDevice(), _imageView, nullptr);
    vkDestroyImage(_device->GetVirtualDevice(), _deviceImage, nullptr);
    vkFreeMemory(_device->GetVirtualDevice(), _deviceImageMemory, nullptr);
}

void VulkanImage::UpdateImageData(Buffer *stagingBuffer)
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
    vkCmdCopyBufferToImage(commandBuffer, stagingBuffer->GetBuffer(), _deviceImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

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

        if (_imageType == ImageType::Color)
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }
        else if (_imageType == ImageType::Depth)
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            if (HasStencilComponent(_imageFormat))
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }

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
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
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
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
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

bool VulkanImage::HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat VulkanImage::FindDepthFormat(Device *device)
{
    return FindSupportedFormat(
        {VK_FORMAT_D32_SFLOAT,
         VK_FORMAT_D32_SFLOAT_S8_UINT,
         VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, device);
}

VkFormat VulkanImage::FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features, Device *device)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(device->GetPhysicalDevice(), format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    Console::Fatal("Format not supported!");
    return {};
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga