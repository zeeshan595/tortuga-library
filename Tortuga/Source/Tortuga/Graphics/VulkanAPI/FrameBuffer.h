#ifndef _VULKAN_API_FRAME_BUFFER
#define _VULKAN_API_FRAME_BUFFER

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "Pipeline.h"
#include "VulkanImage.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class FrameBuffer
{
private:
  Device *_device;
  std::vector<VkFramebuffer> _frameBuffer;

public:
  FrameBuffer(Device *device, Swapchain *swapchain, VulkanImage *depthImage, RenderPass *renderPass);
  ~FrameBuffer();

  std::vector<VkFramebuffer> GetFramebuffers() { return _frameBuffer; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif