#ifndef _VULKAN_IMAGE_VIEW
#define _VULKAN_IMAGE_VIEW

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./Image.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace ImageView
{
struct ImageView
{
  VkDevice Device = VK_NULL_HANDLE;
  VkImage Image = VK_NULL_HANDLE;
  VkImageView View = VK_NULL_HANDLE;
};
ImageView Create(Device::Device device, Image::Image image, VkImageAspectFlags aspectFlags);
ImageView Create(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
void Destroy(ImageView data);
} // namespace ImageView
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif