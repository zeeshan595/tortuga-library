#include "./Rendering.hpp"

#define RENDER_WIDTH 1920
#define RENDER_HEIGHT 1080

namespace Tortuga
{
namespace Systems
{
void Rendering::Update()
{
  Graphics::DisplaySurface::Dispatch(DisplaySurface);

  if (!Graphics::Vulkan::Fence::IsFenceSignaled(PresentFence))
    return;
  Graphics::Vulkan::Fence::ResetFences({PresentFence});

  const auto device = VulkanInstance.Devices[0];
  //render image
  Graphics::Vulkan::Command::Begin(RenderCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  Graphics::Vulkan::Command::BindPipeline(RenderCommand, VK_PIPELINE_BIND_POINT_COMPUTE, RenderPipeline, {RenderOptionsDescriptorSet, RenderDescriptorSet});
  Graphics::Vulkan::Command::Compute(RenderCommand, RENDER_WIDTH / 8, RENDER_HEIGHT / 8, 1);
  Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, RenderedImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  Graphics::Vulkan::Command::BufferToImage(RenderCommand, RenderingBuffer, RenderedImage, {0, 0}, {RenderedImage.Width, RenderedImage.Height});
  Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, RenderedImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  Graphics::Vulkan::Command::End(RenderCommand);
  Graphics::Vulkan::Command::Submit({RenderCommand}, device.Queues.Compute[0], {}, {RenderSemaphore});
  Graphics::Vulkan::Device::WaitForQueue(device.Queues.Compute[0]);

  //present image
  const auto presentImageIndex = Graphics::Vulkan::Swapchain::AquireNextImage(DisplaySurface.Swapchain);
  const auto presentImage = Graphics::Vulkan::Swapchain::GetImage(DisplaySurface.Swapchain, presentImageIndex);
  Graphics::Vulkan::Command::Begin(PresentCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  Graphics::Vulkan::Command::TransferImageLayout(PresentCommand, presentImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  Graphics::Vulkan::Command::BlitImage(PresentCommand, RenderedImage, presentImage);
  Graphics::Vulkan::Command::TransferImageLayout(PresentCommand, presentImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
  Graphics::Vulkan::Command::End(PresentCommand);
  Graphics::Vulkan::Command::Submit({PresentCommand}, device.Queues.Graphics[0], {RenderSemaphore}, {PresentSemaphore}, PresentFence);
  Graphics::Vulkan::Swapchain::PresentImage(DisplaySurface.Swapchain, presentImageIndex, device.Queues.Present, {PresentSemaphore});
}
Rendering::Rendering()
{
  VulkanInstance = Graphics::Vulkan::Instance::Create();
  const auto device = VulkanInstance.Devices[0];
  DisplaySurface = Graphics::DisplaySurface::Create(VulkanInstance, device);
  GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  ComputeCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
  TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);

  //layout information
  DescriptorLayouts.clear();
  DescriptorLayouts.push_back(Graphics::Vulkan::DescriptorLayout::Create(device, 1, VK_SHADER_STAGE_COMPUTE_BIT, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
  DescriptorLayouts.push_back(Graphics::Vulkan::DescriptorLayout::Create(device, 1, VK_SHADER_STAGE_COMPUTE_BIT, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER));

  //shader code
  const auto shaderCode = Utils::IO::GetFileContents("Assets/Shaders/Renderer.comp");
  const auto compiledShaderCode = Graphics::Vulkan::Shader::CompileShader(VulkanInstance, Graphics::Vulkan::Shader::COMPUTE, shaderCode);
  RenderShader = Graphics::Vulkan::Shader::Create(device, compiledShaderCode);
  RenderPipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, RenderShader, {}, DescriptorLayouts);

  //rendering options
  RenderOptionsBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, sizeof(RenderOptions), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  {
    const auto stagingRenderOptionsBuffer = Graphics::Vulkan::Buffer::CreateHost(device, sizeof(RenderOptions), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    RenderOptions data = {};
    {
      data.renderWidth = RENDER_WIDTH;
      data.renderHeight = RENDER_HEIGHT;
    }
    Graphics::Vulkan::Buffer::SetData(stagingRenderOptionsBuffer, &data, stagingRenderOptionsBuffer.Size);
    const auto command = Graphics::Vulkan::Command::Create(device, TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
    Graphics::Vulkan::Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::Vulkan::Command::CopyBuffer(command, stagingRenderOptionsBuffer, RenderOptionsBuffer);
    Graphics::Vulkan::Command::End(command);
    Graphics::Vulkan::Command::Submit({command}, device.Queues.Transfer[0]);
    Graphics::Vulkan::Device::WaitForQueue(device.Queues.Transfer[0]);
    Graphics::Vulkan::Buffer::Destroy(stagingRenderOptionsBuffer);
  }
  RenderOptionsDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {DescriptorLayouts[0]});
  RenderOptionsDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, RenderOptionsDescriptorPool, DescriptorLayouts[0]);
  Graphics::Vulkan::DescriptorSet::UpdateDescriptorSet(RenderOptionsDescriptorSet, {RenderOptionsBuffer});

  //rendering buffer
  RenderingBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, sizeof(glm::vec4) * RENDER_WIDTH * RENDER_HEIGHT, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  RenderDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {DescriptorLayouts[1]});
  RenderDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, RenderDescriptorPool, {DescriptorLayouts[1]});
  Graphics::Vulkan::DescriptorSet::UpdateDescriptorSet(RenderDescriptorSet, {RenderingBuffer});

  //rendered image
  RenderedImage = Graphics::Vulkan::Image::Create(device, RENDER_WIDTH, RENDER_HEIGHT, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);

  //Commands
  RenderCommand = Graphics::Vulkan::Command::Create(device, ComputeCommandPool, Graphics::Vulkan::Command::PRIMARY);
  PresentCommand = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);

  RenderSemaphore = Graphics::Vulkan::Semaphore::Create(device);
  PresentSemaphore = Graphics::Vulkan::Semaphore::Create(device);
  PresentFence = Graphics::Vulkan::Fence::Create(device, true);
}
Rendering::~Rendering()
{
  Graphics::Vulkan::Semaphore::Destroy(RenderSemaphore);
  Graphics::Vulkan::Semaphore::Destroy(PresentSemaphore);
  Graphics::Vulkan::Fence::Destroy(PresentFence);

  Graphics::Vulkan::DescriptorPool::Destroy(RenderDescriptorPool);
  Graphics::Vulkan::DescriptorPool::Destroy(RenderOptionsDescriptorPool);
  Graphics::Vulkan::Buffer::Destroy(RenderOptionsBuffer);
  Graphics::Vulkan::Buffer::Destroy(RenderingBuffer);

  Graphics::Vulkan::Pipeline::Destroy(RenderPipeline);
  Graphics::Vulkan::Shader::Destroy(RenderShader);
  for (const auto layout : DescriptorLayouts)
    Graphics::Vulkan::DescriptorLayout::Destroy(layout);
  DescriptorLayouts.clear();

  Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  Graphics::Vulkan::CommandPool::Destroy(ComputeCommandPool);
  Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
  Graphics::DisplaySurface::Destroy(DisplaySurface);
  Graphics::Vulkan::Instance::Destroy(VulkanInstance);
}
} // namespace Systems
} // namespace Tortuga