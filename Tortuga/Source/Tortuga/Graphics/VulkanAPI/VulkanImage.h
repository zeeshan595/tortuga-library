#ifndef _VULKAN_API_IMAGE
#define _VULKAN_API_IMAGE

#include "../../Core.h"
#include "../../Console.h"

#include "Device.h"
#include "CommandPool.h"
#include "StorageType.h"
#include "Buffer.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class VulkanImage
{
public:
  enum ImageType
  {
    Color,
    Depth
  };

private:
  uint32_t _width;
  uint32_t _height;
  Device *_device;
  ImageType _imageType;

  VkImage _deviceImage;
  VkDeviceMemory _deviceImageMemory;

  VkImageView _imageView;
  VkSampler _textureSampler;

  uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags properties);
  void TransitionImageLayout(VkCommandBuffer commandBuffer,
                             VkImage image,
                             VkFormat format,
                             VkImageLayout oldLayout,
                             VkImageLayout newLayout);
  void CreateImage(uint32_t width,
                   uint32_t height,
                   VkFormat format,
                   VkImageTiling tiling,
                   VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties,
                   VkImage &image,
                   VkDeviceMemory &imageMemory);

  VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
  VkFormat FindDepthFormat();
  bool HasStencilComponent(VkFormat format);

public:
  VulkanImage(Device *device, uint32_t width, uint32_t height);
  ~VulkanImage();

  void UpdateImageData(Buffer *stagingBuffer);

  VkImageView GetImageView() { return _imageView; }
  VkSampler GetSampler() { return _textureSampler; }
  VkImage GetImage() { return _deviceImage; }
  VkDeviceMemory GetMemory() { return _deviceImageMemory; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif