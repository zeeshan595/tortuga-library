#ifndef _FRAME_BUFFERS
#define _FRAME_BUFFERS

#include "VulkanAPI/FrameBuffers.h"

#include "HardwareController.h"
#include "RenderPass.h"
#include "VulkanAPI/VulkanImage.h"

namespace Tortuga
{
namespace Graphics
{
struct FrameBuffer
{
  std::vector<VulkanAPI::VulkanImageData> Images;
  std::vector<VulkanAPI::FrameBufferData> VulkanFrameBuffers;
};
FrameBuffer CreateFrameBuffers(HardwareController hardware, RenderPass renderPass);
void DestroyFrameBuffers(FrameBuffer data);
}; // namespace Graphics
}; // namespace Tortuga

#endif