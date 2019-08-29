#ifndef _VULKAN_RENDER_PASS
#define _VULKAN_RENDER_PASS

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Image.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace RenderPass
{
struct RenderPass 
{
  VkDevice Device = VK_NULL_HANDLE;
  VkRenderPass RenderPass = VK_NULL_HANDLE;
  std::vector<VkAttachmentDescription> Attachments;
};

RenderPass Create(Device::Device device, VkFormat imageFormat);
void Destroy(RenderPass data);
} // namespace Image
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif