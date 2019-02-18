#include "Renderer.h"

namespace Tortuga
{
namespace Graphics
{
void SubmitCommands(Renderer data, std::vector<CommandBuffer> commands)
{
  for (uint32_t i = 0; i < data.VulkanRenderers.size(); i++)
  {
    std::vector<VulkanAPI::CommandBufferData> vulkanCommands(commands.size());
    for (uint32_t j = 0; j < vulkanCommands.size(); j++)
      vulkanCommands[j] = commands[j].CommandBuffers[i];

    VulkanAPI::SubmitCommands(data.VulkanRenderers[i], vulkanCommands);
  }
}
void DrawFrame(Renderer data)
{
  for (uint32_t i = 0; i < data.VulkanRenderers.size(); i++)
  {
    VulkanAPI::DrawFrame(data.VulkanRenderers[i]);
  }
  for (uint32_t i = 0; i < data.VulkanRenderers.size(); i++)
  {
    vkDeviceWaitIdle(data.VulkanDevices[i].Device);
  }
}
Renderer CreateRenderer(Window window, FrameBuffer frameBuffer, RenderPass renderPass)
{
  auto data = Renderer();
  data.VulkanDevices = window.VulkanDevicesInUse;

  data.VulkanRenderers.resize(window.VulkanDevicesInUse.size());
  for (uint32_t i = 0; i < window.VulkanDevicesInUse.size(); i++)
  {
    data.VulkanRenderers[i] = VulkanAPI::CreateRenderer(
        window.VulkanDevicesInUse[i],
        window.VulkanSwapchain[i],
        frameBuffer.VulkanFrameBuffers[i],
        renderPass.VulkanRenderPass[i]);
  }

  return data;
}
void DestroyRenderer(Renderer data)
{
  for (uint32_t i = 0; i < data.VulkanRenderers.size(); i++)
  {
    VulkanAPI::DestroyRenderer(data.VulkanRenderers[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga
