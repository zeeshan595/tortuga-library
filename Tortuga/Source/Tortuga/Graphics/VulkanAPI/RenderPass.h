#ifndef _RENDER_PASS
#define _RENDER_PASS

#include "../../Core.h"
#include "../../Core.h"
#include "Device.h"
#include "Swapchain.h"

namespace Tortuga
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
}; // namespace Tortuga

#endif