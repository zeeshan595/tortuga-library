#ifndef _RENDER_PASS
#define _RENDER_PASS

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "Swapchain.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class RenderPass
{
private:
  Device *_device;
  VkRenderPass _renderPass;

public:
  RenderPass(Device *device, Swapchain *swapchain);
  ~RenderPass();

  VkRenderPass GetRenderPass() { return _renderPass; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif