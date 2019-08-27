#include "./Rendering.hpp"

namespace Tortuga
{
namespace Systems
{
void Rendering::Update()
{
}
Rendering::Rendering()
{
  auto vulkan = Core::Engine::GetVulkan();
  auto device = Core::Engine::GetMainDevice();
  auto swapchain = Core::Screen::GetSwapchain();

  //general
  {
    TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
    ComputeCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
    GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  }

  //graphics pipeline
  {
    auto vertexCode = Utils::IO::GetFileContents("Shaders/simple.vert");
    auto vertexCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::VERTEX, vertexCode);
    auto fragmentCode = Utils::IO::GetFileContents("Shaders/simple.vert");
    auto fragmentCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::FRAGMENT, fragmentCode);

    VertexShader = Graphics::Vulkan::Shader::Create(device, vertexCompile);
    FragmentShader = Graphics::Vulkan::Shader::Create(device, fragmentCompile);
    RenderPass = Graphics::Vulkan::RenderPass::Create(device, swapchain.SurfaceFormat.format);
    Pipeline = Graphics::Vulkan::Pipeline::CreateGraphicsPipeline(device, VertexShader, FragmentShader, RenderPass, swapchain.Extent.width, swapchain.Extent.height);

    Framebuffers.resize(swapchain.Images.size());
    for (uint32_t i = 0; i < Framebuffers.size(); i++)
      Framebuffers[i] = Graphics::Vulkan::Framebuffer::Create(device, swapchain.Extent.width, swapchain.Extent.height, RenderPass, {swapchain.Views[i]});
  }
}
Rendering::~Rendering()
{
  auto vulkan = Core::Engine::GetVulkan();
  auto device = Core::Engine::GetMainDevice();

  Graphics::Vulkan::Device::WaitForDevice(device);

  //general
  {
    Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
    Graphics::Vulkan::CommandPool::Destroy(ComputeCommandPool);
    Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  }

  //graphics pipeline
  {
    Graphics::Vulkan::Pipeline::Destroy(Pipeline);
    Graphics::Vulkan::RenderPass::Destroy(RenderPass);
    Graphics::Vulkan::Shader::Destroy(FragmentShader);
    Graphics::Vulkan::Shader::Destroy(VertexShader);
  }
}
} // namespace Systems
} // namespace Tortuga
