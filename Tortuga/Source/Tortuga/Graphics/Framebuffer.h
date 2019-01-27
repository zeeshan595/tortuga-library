#ifndef _FRAMEBUFFER
#define _FRAMEBUFFER

#include "../Core.h"
#include "../Console.h"
#include "Device.h"
#include "Pipeline.h"

namespace Tortuga
{
class Framebuffer
{
  private:
    Device *_device;
    VkFramebuffer _frameBuffer;

  public:
    Framebuffer(Pipeline *pipeline, std::vector<VkImageView> images);
    ~Framebuffer();

    VkFramebuffer GetFramebuffer() { return _frameBuffer; }
};
}; // namespace Tortuga

#endif