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
Renderer CreateRenderer(HardwareController hardware, FrameBuffer frameBuffer, RenderPass renderPass)
{
  auto data = Renderer();
  data.Hardware = hardware;

  data.VulkanRenderers.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.VulkanRenderers[i] = VulkanAPI::CreateRenderer(
        hardware.Devices[i].VulkanDevice,
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
