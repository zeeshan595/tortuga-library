#ifndef _RENDERER
#define _RENDERER

#include "VulkanAPI/Device.h"
#include "VulkanAPI/Renderer.h"

#include "HardwareController.h"
#include "FrameBuffers.h"
#include "RenderPass.h"
#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
struct Renderer
{
  HardwareController Hardware;
  std::vector<VulkanAPI::RendererData> VulkanRenderers;
};
void SubmitCommands(Renderer data, std::vector<CommandBuffer> commands);
void DrawFrame(Renderer data);
Renderer CreateRenderer(HardwareController hardware, FrameBuffer frameBuffer, RenderPass renderPass);
void DestroyRenderer(Renderer data);
}; // namespace Graphics
}; // namespace Tortuga

#endif