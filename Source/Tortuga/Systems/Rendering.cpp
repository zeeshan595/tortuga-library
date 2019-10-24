#include "./Rendering.hpp"

namespace Tortuga
{
namespace Systems
{
Rendering::Rendering()
{
  //vulkan instance & display surface
  VulkanInstance = Graphics::Vulkan::Instance::Create();
  const auto device = VulkanInstance.Devices[0];
  DisplaySurface = Graphics::DisplaySurface::Create(VulkanInstance, VulkanInstance.Devices[0]);

  //command pools
  TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
  ComputeCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
  GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);

  //setup descriptor set layouts
  //DescriptorLayouts.push_back(Graphics::Vulkan::DescriptorLayout::Create(device, {VK_SHADER_STAGE_VERTEX_BIT}, {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}));

  //render pass & pipeline
  {
    //vertex shader
    const auto vertexShader = Graphics::Vulkan::Shader::GetFullShaderCode("Assets/Shaders/simple.vert");
    const auto compiledVertex = Graphics::Vulkan::Shader::CompileShader(vertexShader.code, vertexShader.location, vertexShader.type);
    Shaders.push_back(Graphics::Vulkan::Shader::Create(device, compiledVertex, VK_SHADER_STAGE_VERTEX_BIT));

    //fragment shader
    const auto fragmentShader = Graphics::Vulkan::Shader::GetFullShaderCode("Assets/Shaders/simple.frag");
    const auto compiledFragment = Graphics::Vulkan::Shader::CompileShader(fragmentShader.code, fragmentShader.location, fragmentShader.type);
    Shaders.push_back(Graphics::Vulkan::Shader::Create(device, compiledFragment, VK_SHADER_STAGE_FRAGMENT_BIT));
  }
  RenderPass = Graphics::Vulkan::RenderPass::Create(device);
  Pipeline = Graphics::Vulkan::Pipeline::CreateGraphicsPipeline(device, Shaders, RenderPass, Graphics::Vertex::GetBindingDescription(), Graphics::Vertex::GetAttributeDescriptions());

  //render image
  RenderImage = Graphics::Vulkan::Image::Create(device, 1920, 1080, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
  RenderImageView = Graphics::Vulkan::ImageView::Create(device, RenderImage, VK_IMAGE_ASPECT_COLOR_BIT);
  RenderDepthImage = Graphics::Vulkan::Image::Create(device, RenderImage.Width, RenderImage.Height, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
  RenderDepthImageView = Graphics::Vulkan::ImageView::Create(device, RenderDepthImage, VK_IMAGE_ASPECT_DEPTH_BIT);
  Framebuffer = Graphics::Vulkan::Framebuffer::Create(device, RenderImage.Width, RenderImage.Height, RenderPass, {RenderImageView, RenderDepthImageView});
  RenderCommand = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::Type::PRIMARY);
}
Rendering::~Rendering()
{
  Graphics::Vulkan::Image::Destroy(RenderImage);

  Graphics::Vulkan::Pipeline::Destroy(Pipeline);
  Graphics::Vulkan::RenderPass::Destroy(RenderPass);

  for (const auto shader : Shaders)
    Graphics::Vulkan::Shader::Destroy(shader);

  for (const auto descriptorLayout : DescriptorLayouts)
    Graphics::Vulkan::DescriptorLayout::Destroy(descriptorLayout);

  Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  Graphics::DisplaySurface::Destroy(DisplaySurface);
  Graphics::Vulkan::Instance::Destroy(VulkanInstance);
}
void Rendering::Update()
{
  const auto device = VulkanInstance.Devices[0];
  Graphics::DisplaySurface::Dispatch(DisplaySurface);

  const auto cameras = Core::Engine::GetComponents<Components::Camera>();
  for (const auto camera : cameras)
  {
    //record render commnad
    Graphics::Vulkan::Command::Begin(RenderCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, RenderImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, RenderDepthImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    //run all draw commands
    Graphics::Vulkan::Command::BeginRenderPass(RenderCommand, RenderPass, Framebuffer, Framebuffer.Width, Framebuffer.Height);

    //draw command
    std::vector<std::future<Graphics::Vulkan::Command::Command>> meshThreads;
    const auto meshViews = Core::Engine::GetComponents<MeshView>();
    for (const auto meshView : meshViews)
    {
      const auto renderCommand = RenderCommand;
      const auto renderPass = RenderPass;
      const auto framebuffer = Framebuffer;
      const auto pipeline = Pipeline;
      meshThreads.push_back(std::async(std::launch::async, [device, meshView, renderPass, framebuffer, pipeline] {
        meshView->Setup(device);
        Graphics::Vulkan::Command::Begin(meshView->RenderCommand, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, renderPass, 0, framebuffer);
        Graphics::Vulkan::Command::CopyBuffer(meshView->RenderCommand, meshView->StagingVertexBuffer, meshView->VertexBuffer);
        Graphics::Vulkan::Command::CopyBuffer(meshView->RenderCommand, meshView->StagingIndexBuffer, meshView->IndexBuffer);
        Graphics::Vulkan::Command::BindPipeline(meshView->RenderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline, {});
        Graphics::Vulkan::Command::SetViewport(meshView->RenderCommand, 0, 0, 1920, 1080);
        Graphics::Vulkan::Command::BindVertexBuffer(meshView->RenderCommand, {meshView->VertexBuffer});
        Graphics::Vulkan::Command::BindIndexBuffer(meshView->RenderCommand, meshView->IndexBuffer);
        Graphics::Vulkan::Command::DrawIndexed(meshView->RenderCommand, meshView->IndexCount);
        Graphics::Vulkan::Command::End(meshView->RenderCommand);
        return meshView->RenderCommand;
      }));
    }

    std::vector<Graphics::Vulkan::Command::Command> drawCommands;
    for (uint32_t i = 0; i < meshThreads.size(); i++)
    {
      meshThreads[i].wait();
      drawCommands.push_back(meshThreads[i].get());
    }

    Graphics::Vulkan::Command::ExecuteCommands(RenderCommand, drawCommands);
    Graphics::Vulkan::Command::EndRenderPass(RenderCommand);

    Graphics::Vulkan::Command::End(RenderCommand);
  }

  Graphics::Vulkan::Device::WaitForQueue(device.Queues.Graphics[0]);
}

void Rendering::MeshView::Setup(Graphics::Vulkan::Device::Device device)
{
  if (this->Root == nullptr)
    return;

  const auto mesh = Core::Engine::GetComponent<Components::Mesh>(this->Root);
  if (mesh == nullptr)
    return;

  this->IsStatic = mesh->IsStatic;
  const uint32_t verticesSize = sizeof(Graphics::Vertex) * mesh->Vertices.size();
  const uint32_t indicesSize = sizeof(uint32_t) * mesh->Indices.size();

  if (!this->IsStatic)
  {
    const auto transform = Core::Engine::GetComponent<Components::Transform>(this->Root);
    if (transform != nullptr)
      this->Transform = transform->GetModelMatrix();
  }

  bool IsUpdated = false;
  if (this->StagingVertexBuffer.Buffer == VK_NULL_HANDLE || mesh->IsVerticesDirty)
  {
    if (this->StagingVertexBuffer.Buffer == VK_NULL_HANDLE || this->StagingVertexBuffer.Size != verticesSize)
    {
      Graphics::Vulkan::Buffer::Destroy(this->StagingVertexBuffer);
      Graphics::Vulkan::Buffer::Destroy(this->VertexBuffer);
    }
    this->StagingVertexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    this->VertexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    Graphics::Vulkan::Buffer::SetData(this->StagingVertexBuffer, mesh->Vertices.data(), verticesSize);
    IsUpdated = true;
  }

  if (this->StagingIndexBuffer.Buffer == VK_NULL_HANDLE || mesh->IsIndicesDirty)
  {
    if (this->StagingIndexBuffer.Buffer == VK_NULL_HANDLE || this->StagingIndexBuffer.Size != indicesSize)
    {
      Graphics::Vulkan::Buffer::Destroy(this->StagingIndexBuffer);
      Graphics::Vulkan::Buffer::Destroy(this->IndexBuffer);
    }
    this->StagingIndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    this->IndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    Graphics::Vulkan::Buffer::SetData(this->StagingIndexBuffer, mesh->Indices.data(), indicesSize);
    this->IndexCount = mesh->Indices.size();
    IsUpdated = true;
  }

  if (!IsUpdated)
    return;

  if (this->CommandPool.CommandPool == VK_NULL_HANDLE)
    this->CommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  if (this->RenderCommand.Command == VK_NULL_HANDLE)
    this->RenderCommand = Graphics::Vulkan::Command::Create(device, this->CommandPool, Graphics::Vulkan::Command::PRIMARY);
}
void Rendering::MeshView::OnDestroy()
{
  Graphics::Vulkan::Buffer::Destroy(StagingVertexBuffer);
  Graphics::Vulkan::Buffer::Destroy(VertexBuffer);
  Graphics::Vulkan::Buffer::Destroy(StagingIndexBuffer);
  Graphics::Vulkan::Buffer::Destroy(IndexBuffer);
  Graphics::Vulkan::CommandPool::Destroy(CommandPool);
}
} // namespace Systems
} // namespace Tortuga