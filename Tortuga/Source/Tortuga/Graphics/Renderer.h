#ifndef _RENDERER
#define _RENDERER

#include "VulkanAPI/Device.h"
#include "VulkanAPI/Renderer.h"

#include "Window.h"
#include "FrameBuffers.h"
#include "RenderPass.h"
#include "CommandBuffer.h"

namespace Tortuga
{
namespace Graphics
{
struct Renderer
{
  std::vector<VulkanAPI::DeviceData> VulkanDevices;
  std::vector<VulkanAPI::RendererData> VulkanRenderers;
};
void SubmitCommands(Renderer data, std::vector<CommandBuffer> commands);
void DrawFrame(Renderer data);
Renderer CreateRenderer(Window window, FrameBuffer frameBuffer, RenderPass renderPass);
void DestroyRenderer(Renderer data);
}; // namespace Graphics
}; // namespace Tortuga

#endif