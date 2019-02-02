#ifndef _FRAME_BUFFER
#define _FRAME_BUFFER

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "Pipeline.h"

namespace Tortuga
{
class FrameBuffer
{
  private:
    Device *_device;
    VkFramebuffer _frameBuffer;

  public:
    FrameBuffer(Device *device, Swapchain *swapchain, RenderPass *renderPass, std::vector<VkImageView> images);
    ~FrameBuffer();

    VkFramebuffer GetFramebuffer() { return _frameBuffer; }
};
}; // namespace Tortuga

#endif