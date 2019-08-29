#ifndef _VULKAN_FRAMEBUFFER
#define _VULKAN_FRAMEBUFFER

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./RenderPass.hpp"
#include "./ImageView.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Framebuffer
{
struct Framebuffer 
{
  VkDevice Device = VK_NULL_HANDLE;
  VkFramebuffer Framebuffer = VK_NULL_HANDLE;
  uint32_t Width;
  uint32_t Height;
};

Framebuffer Create(Device::Device device, uint32_t width, uint32_t height, RenderPass::RenderPass renderPass, std::vector<ImageView::ImageView> imageViews);
void Destroy(Framebuffer data);
} // namespace Image
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif