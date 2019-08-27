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

  const auto device = Core::Engine::GetMainDevice();
  const auto swapchain = Core::Screen::GetSwapchain();

  //get current swapchain image being rendered
  const auto swapchainIndex = Graphics::Vulkan::Swapchain::AquireNextImage(swapchain);

  //start recording primary command for gpu
  Graphics::Vulkan::Command::Begin(Renderer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  Graphics::Vulkan::Command::BeginRenderPass(Renderer, RenderPass, Framebuffers[swapchainIndex], swapchain.Extent.width, swapchain.Extent.height);

  //start recording data transfer command
  Graphics::Vulkan::Command::Begin(Transfer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

  //record a sub-command for each mesh
  std::vector<Graphics::Vulkan::Command::Command> renderCommands;
  std::vector<std::future<void>> vulkanThreads;
  for (auto entity : Core::Entity::GetAllEntities())
  {
    auto mesh = entity->GetComponent<Component::Mesh>();
    if (mesh != nullptr)
    {
      //make sure mesh's render sub-command is setup
      if (mesh->RenderCommand.Command == VK_NULL_HANDLE)
        mesh->RenderCommand = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::SECONDARY);
      renderCommands.push_back(mesh->RenderCommand);

      //thread data
      const auto pipeline = Pipeline;
      const auto renderPass = RenderPass;
      const auto framebuffer = Framebuffers[swapchainIndex];
      const auto transferCommand = Transfer;
      vulkanThreads.push_back(std::async(std::launch::async, [mesh, pipeline, renderPass, framebuffer, device, transferCommand] {
        //record mesh sub-command
        Graphics::Vulkan::Command::Begin(mesh->RenderCommand, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, renderPass, framebuffer);

        //make mesh data is up to date
        uint vertexBufferSize = mesh->Vertices.size() * sizeof(Graphics::Vertex);
        if (mesh->StagingVertexBuffer.Buffer == VK_NULL_HANDLE || vertexBufferSize != mesh->StagingVertexBuffer.Size)
        {
          //delete old buffers if they exist
          if (mesh->StagingVertexBuffer.Buffer != VK_NULL_HANDLE)
          {
            Graphics::Vulkan::Buffer::Destroy(mesh->StagingVertexBuffer);
            Graphics::Vulkan::Buffer::Destroy(mesh->VertexBuffer);
          }
          //create new buffers
          mesh->StagingVertexBuffer = Graphics::Vulkan::Buffer::Create(device, vertexBufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
          mesh->VertexBuffer = Graphics::Vulkan::Buffer::Create(device, vertexBufferSize, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
          //copy data
          Graphics::Vulkan::Buffer::SetData(mesh->StagingVertexBuffer, mesh->Vertices.data(), vertexBufferSize);
          //data needs to be copied by transfer command
          Graphics::Vulkan::Command::CopyBuffer(transferCommand, mesh->StagingVertexBuffer, mesh->VertexBuffer);
        }

        Graphics::Vulkan::Command::BindPipeline(mesh->RenderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline, {});
        Graphics::Vulkan::Command::BindVertexBuffer(mesh->RenderCommand, {mesh->VertexBuffer});
        Graphics::Vulkan::Command::Draw(mesh->RenderCommand, mesh->Vertices.size());
        Graphics::Vulkan::Command::End(mesh->RenderCommand);
      }));
    }
  }
  //wait for all threads to complete
  for (uint32_t i = 0; i < vulkanThreads.size(); i++)
    vulkanThreads[i].wait();

  Graphics::Vulkan::Command::End(Transfer);
  Graphics::Vulkan::Command::Submit({Transfer}, device.Queues.Transfer[0], {}, {RenderSemaphore});

  //execute all sub-commands
  Graphics::Vulkan::Command::ExecuteCommands(Renderer, renderCommands);
  Graphics::Vulkan::Command::EndRenderPass(Renderer);
  Graphics::Vulkan::Command::End(Renderer);

  //submit primary command
  Graphics::Vulkan::Command::Submit({Renderer}, device.Queues.Graphics[0], {RenderSemaphore}, {PresentSemaphore}, RenderFence);
  //present the image
  Graphics::Vulkan::Swapchain::PresentImage(swapchain, swapchainIndex, device.Queues.Graphics[0], {PresentSemaphore});
}
Rendering::Rendering()
{
  const auto vulkan = Core::Engine::GetVulkan();
  const auto device = Core::Engine::GetMainDevice();
  const auto swapchain = Core::Screen::GetSwapchain();

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
    Pipeline = Graphics::Vulkan::Pipeline::CreateGraphicsPipeline(device, VertexShader, FragmentShader, RenderPass, swapchain.Extent.width, swapchain.Extent.height, {Graphics::Vertex::getBindingDescription()}, Graphics::Vertex::getAttributeDescriptions());

    Framebuffers.resize(swapchain.Images.size());
    for (uint32_t i = 0; i < Framebuffers.size(); i++)
      Framebuffers[i] = Graphics::Vulkan::Framebuffer::Create(device, swapchain.Extent.width, swapchain.Extent.height, RenderPass, {swapchain.Views[i]});

    Renderer = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);
    Transfer = Graphics::Vulkan::Command::Create(device, TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
    RenderFence = Graphics::Vulkan::Fence::Create(device, true);
    RenderSemaphore = Graphics::Vulkan::Semaphore::Create(device);
    PresentSemaphore = Graphics::Vulkan::Semaphore::Create(device);
  }
}

Rendering::~Rendering()
{
  const auto vulkan = Core::Engine::GetVulkan();
  const auto device = Core::Engine::GetMainDevice();

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
    Graphics::Vulkan::Semaphore::Destroy(RenderSemaphore);
    Graphics::Vulkan::Semaphore::Destroy(PresentSemaphore);
    Graphics::Vulkan::Pipeline::Destroy(Pipeline);
    Graphics::Vulkan::RenderPass::Destroy(RenderPass);
    Graphics::Vulkan::Shader::Destroy(FragmentShader);
    Graphics::Vulkan::Shader::Destroy(VertexShader);
  }
}
} // namespace Systems
} // namespace Tortuga
