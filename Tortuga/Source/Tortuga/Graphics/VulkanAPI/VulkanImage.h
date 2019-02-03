#ifndef _VULKAN_IMAGE
#define _VULKAN_IMAGE

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
  uint32_t _size;
  Device *_device;
  ImageType _imageType;
  VkDeviceSize _imageSize;

  VkBuffer _stagingBuffer;
  VkDeviceMemory _stangingMemory;

  VkImage _deviceImage;
  VkDeviceMemory _deviceImageMemory;

  void CopyToDevice(SDL_Surface *image);
  uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags properties);
  void TransitionImageLayout(VkCommandBuffer commandBuffer,
                             VkImage image,
                             VkFormat format,
                             VkImageLayout oldLayout,
                             VkImageLayout newLayout);
  void CreateBuffer(VkBufferUsageFlags flags,
                    VkMemoryPropertyFlags memoryProperties,
                    VkBuffer &handler,
                    VkDeviceMemory &deviceMemory);
  void CreateImage(uint32_t width,
                   uint32_t height,
                   VkFormat format,
                   VkImageTiling tiling,
                   VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties,
                   VkImage &image,
                   VkDeviceMemory &imageMemory);

public:
  VulkanImage(Device *device, uint32_t width, uint32_t height, ImageType imageType, uint32_t size);
  ~VulkanImage();

  void UpdateImageData(SDL_Surface *image);
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif