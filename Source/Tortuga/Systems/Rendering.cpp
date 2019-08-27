#include "./Rendering.hpp"

namespace Tortuga
{
namespace Systems
{
void Rendering::Update()
{
  //check if rendering is already in progress
  if (!Graphics::Vulkan::Fence::IsFenceSignaled(RenderFence))
    return;
  Graphics::Vulkan::Fence::ResetFences({RenderFence});

  auto device = Core::Engine::GetMainDevice();
  auto swapchain = Core::Screen::GetSwapchain();

  //get current swapchain image being rendered
  auto swapchainIndex = Graphics::Vulkan::Swapchain::AquireNextImage(swapchain);

  //start recording primary command for gpu
  Graphics::Vulkan::Command::Begin(Renderer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  Graphics::Vulkan::Command::BeginRenderPass(Renderer, RenderPass, Framebuffers[swapchainIndex], swapchain.Extent.width, swapchain.Extent.height);

  //record a sub-command for each mesh
  std::vector<Graphics::Vulkan::Command::Command> vulkanCommands;
  std::vector<std::future<void>> vulkanThreads;
  for (auto entity : Core::Entity::GetAllEntities())
  {
    auto mesh = entity->GetComponent<Component::Mesh>();
    if (mesh != nullptr)
    {
      //make sure mesh's render sub-command is setup
      if (mesh->RenderCommand.Command == VK_NULL_HANDLE)
        mesh->RenderCommand = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::SECONDARY);
      vulkanCommands.push_back(mesh->RenderCommand);

      //thread data
      auto pipeline = Pipeline;
      auto renderPass = RenderPass;
      auto framebuffer = Framebuffers[swapchainIndex];
      vulkanThreads.push_back(std::async(std::launch::async, [mesh, pipeline, renderPass, framebuffer] {
        //record mesh sub-command
        Graphics::Vulkan::Command::Begin(mesh->RenderCommand, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, renderPass, framebuffer);
        Graphics::Vulkan::Command::BindPipeline(mesh->RenderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline, {});
        Graphics::Vulkan::Command::Draw(mesh->RenderCommand, 3);
        Graphics::Vulkan::Command::End(mesh->RenderCommand);
      }));
    }
  }
  //wait for all threads to complete
  for (uint32_t i = 0; i < vulkanThreads.size(); i++)
    vulkanThreads[i].wait();

  //execute all sub-commands
  Graphics::Vulkan::Command::ExecuteCommands(Renderer, vulkanCommands);
  Graphics::Vulkan::Command::EndRenderPass(Renderer);
  Graphics::Vulkan::Command::End(Renderer);

  //submit primary command
  Graphics::Vulkan::Command::Submit({Renderer}, device.Queues.Graphics[0], {}, {PresentSemaphore}, RenderFence);
  //present the image
  Graphics::Vulkan::Swapchain::PresentImage(swapchain, swapchainIndex, device.Queues.Graphics[0], {PresentSemaphore});
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
    auto fragmentCode = Utils::IO::GetFileContents("Shaders/simple.frag");
    auto fragmentCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::FRAGMENT, fragmentCode);

    VertexShader = Graphics::Vulkan::Shader::Create(device, vertexCompile);
    FragmentShader = Graphics::Vulkan::Shader::Create(device, fragmentCompile);
    RenderPass = Graphics::Vulkan::RenderPass::Create(device, swapchain.SurfaceFormat.format);
    Pipeline = Graphics::Vulkan::Pipeline::CreateGraphicsPipeline(device, VertexShader, FragmentShader, RenderPass, swapchain.Extent.width, swapchain.Extent.height);

    Framebuffers.resize(swapchain.Images.size());
    for (uint32_t i = 0; i < Framebuffers.size(); i++)
      Framebuffers[i] = Graphics::Vulkan::Framebuffer::Create(device, swapchain.Extent.width, swapchain.Extent.height, RenderPass, {swapchain.Views[i]});

    Renderer = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);
    RenderFence = Graphics::Vulkan::Fence::Create(device, true);
    PresentSemaphore = Graphics::Vulkan::Semaphore::Create(device);
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
    Graphics::Vulkan::Fence::Destroy(RenderFence);
    Graphics::Vulkan::Semaphore::Destroy(PresentSemaphore);
    Graphics::Vulkan::Pipeline::Destroy(Pipeline);
    Graphics::Vulkan::RenderPass::Destroy(RenderPass);
    Graphics::Vulkan::Shader::Destroy(FragmentShader);
    Graphics::Vulkan::Shader::Destroy(VertexShader);
  }
}
} // namespace Systems
} // namespace Tortuga
