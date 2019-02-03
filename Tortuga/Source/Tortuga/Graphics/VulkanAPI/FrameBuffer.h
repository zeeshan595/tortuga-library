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
    std::vector<VkFramebuffer> _frameBuffer;

    void CreateDepthImageView();

  public:
    FrameBuffer(Device *device, Swapchain *swapchain, RenderPass *renderPass);
    ~FrameBuffer();

    std::vector<VkFramebuffer> GetFramebuffers() { return _frameBuffer; }
};
}; // namespace Tortuga

#endif