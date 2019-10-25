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
  DescriptorLayouts.push_back(Graphics::Vulkan::DescriptorLayout::Create(device, {VK_SHADER_STAGE_VERTEX_BIT}, {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER}));

  //rendering
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
  RenderCommand = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::Type::PRIMARY);
}
Rendering::~Rendering()
{
  Graphics::Vulkan::Device::WaitForDevice(VulkanInstance.Devices[0]);

  for (const auto fence : RenderFence)
    Graphics::Vulkan::Fence::Destroy(fence);

  for (const auto semaphore : RenderSemaphore)
    Graphics::Vulkan::Semaphore::Destroy(semaphore);

  for (const auto semaphore : TransferSemaphore)
    Graphics::Vulkan::Semaphore::Destroy(semaphore);

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
void Rendering::SetupSemaphores(uint32_t size)
{
  const auto device = VulkanInstance.Devices[0];

  if (size > RenderSemaphore.size())
    RenderSemaphore.resize(size);
  else if (size < RenderSemaphore.size())
  {
    for (uint32_t i = size; i < RenderSemaphore.size(); i++)
      Graphics::Vulkan::Semaphore::Destroy(RenderSemaphore[i]);
  }
  for (uint32_t i = 0; i < size; i++)
  {
    if (RenderSemaphore[i].Semaphore == VK_NULL_HANDLE)
      RenderSemaphore[i] = Graphics::Vulkan::Semaphore::Create(device);
  }

  if (size > TransferSemaphore.size())
    TransferSemaphore.resize(size);
  else if (size < TransferSemaphore.size())
  {
    for (uint32_t i = size; i < TransferSemaphore.size(); i++)
      Graphics::Vulkan::Semaphore::Destroy(TransferSemaphore[i]);
  }
  for (uint32_t i = 0; i < size; i++)
  {
    if (TransferSemaphore[i].Semaphore == VK_NULL_HANDLE)
      TransferSemaphore[i] = Graphics::Vulkan::Semaphore::Create(device);
  }

  if (size > RenderFence.size())
    RenderFence.resize(size);
  else if (size < RenderFence.size())
  {
    for (uint32_t i = size; i < RenderFence.size(); i++)
      Graphics::Vulkan::Fence::Destroy(RenderFence[i]);
  }
  for (uint32_t i = 0; i < size; i++)
  {
    if (RenderFence[i].Fence == VK_NULL_HANDLE)
      RenderFence[i] = Graphics::Vulkan::Fence::Create(device, true);
  }
}
void Rendering::Update()
{
  const auto cameras = Core::Engine::GetComponents<Components::Camera>();
  SetupSemaphores(cameras.size());
  for (const auto fence : RenderFence)
  {
    if (!Graphics::Vulkan::Fence::IsFenceSignaled(fence))
      return;
  }
  Graphics::Vulkan::Fence::ResetFences(RenderFence);

  const auto device = VulkanInstance.Devices[0];
  Graphics::DisplaySurface::Dispatch(DisplaySurface);

  const auto swapchainImageIndex = Graphics::Vulkan::Swapchain::AquireNextImage(DisplaySurface.Swapchain);
  const auto swapchainImage = Graphics::Vulkan::Swapchain::GetImage(DisplaySurface.Swapchain, swapchainImageIndex);

  //make sure every mesh has a mesh view
  const auto meshes = Core::Engine::GetComponents<Components::Mesh>();
  for (const auto mesh : meshes)
  {
    if (Core::Engine::GetComponent<MeshView>(mesh->Root) == nullptr)
      Core::Engine::AddComponent<MeshView>(mesh->Root);
  }
  for (uint32_t cameraIndex = 0; cameraIndex < cameras.size(); cameraIndex++)
  {
    const auto camera = cameras[cameraIndex];
    if (camera->Render.Framebuffer.Framebuffer == VK_NULL_HANDLE)
      camera->Render = Graphics::CameraRender::Create(device, RenderPass, camera->ResolutionWidth, camera->ResolutionHeight);

    //record render commnad
    Graphics::Vulkan::Command::Begin(RenderCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, camera->Render.ColorImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, camera->Render.DepthImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    //run all draw commands
    Graphics::Vulkan::Command::BeginRenderPass(RenderCommand, RenderPass, camera->Render.Framebuffer, camera->Render.Framebuffer.Width, camera->Render.Framebuffer.Height);

    //draw command
    std::vector<std::future<void>> meshThreads;
    const auto meshViews = Core::Engine::GetComponents<MeshView>();
    for (const auto meshView : meshViews)
    {
      const auto renderCommand = RenderCommand;
      const auto renderPass = RenderPass;
      const auto framebuffer = camera->Render.Framebuffer;
      const auto pipeline = Pipeline;
      const auto viewport = camera->Viewport;
      const auto resolutionWidth = camera->ResolutionWidth;
      const auto resolutionHeight = camera->ResolutionHeight;
      meshThreads.push_back(std::async(std::launch::async, [device, meshView, renderPass, framebuffer, pipeline, viewport, resolutionWidth, resolutionHeight] {
        meshView->Setup(device);
        Graphics::Vulkan::Command::Begin(meshView->RenderCommand, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, renderPass, 0, framebuffer);
        Graphics::Vulkan::Command::BindPipeline(meshView->RenderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline, {});
        Graphics::Vulkan::Command::SetViewport(meshView->RenderCommand, viewport.x * resolutionWidth, viewport.y * resolutionHeight, viewport.z * resolutionWidth, viewport.w * resolutionHeight);
        Graphics::Vulkan::Command::BindVertexBuffer(meshView->RenderCommand, {meshView->VertexBuffer});
        Graphics::Vulkan::Command::BindIndexBuffer(meshView->RenderCommand, meshView->IndexBuffer);
        Graphics::Vulkan::Command::DrawIndexed(meshView->RenderCommand, meshView->IndexCount);
        Graphics::Vulkan::Command::End(meshView->RenderCommand);
      }));
    }

    std::vector<Graphics::Vulkan::Command::Command> transferCommands;
    std::vector<Graphics::Vulkan::Command::Command> drawCommands;
    for (uint32_t i = 0; i < meshThreads.size(); i++)
      meshThreads[i].wait();
    for (const auto mesh : meshViews)
    {
      drawCommands.push_back(mesh->RenderCommand);
      transferCommands.push_back(mesh->TransferCommand);
    }
    Graphics::Vulkan::Command::Submit(transferCommands, device.Queues.Transfer[0], {}, {TransferSemaphore[cameraIndex]});

    Graphics::Vulkan::Command::ExecuteCommands(RenderCommand, drawCommands);
    Graphics::Vulkan::Command::EndRenderPass(RenderCommand);
    if (camera->PresentToScreen)
    {
      Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, camera->Render.ColorImage, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
      Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      Graphics::Vulkan::Command::BlitImage(RenderCommand, camera->Render.ColorImage, swapchainImage);
      Graphics::Vulkan::Command::TransferImageLayout(RenderCommand, swapchainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    }
    Graphics::Vulkan::Command::End(RenderCommand);
    Graphics::Vulkan::Command::Submit({RenderCommand}, device.Queues.Graphics[0], {TransferSemaphore[cameraIndex]}, {RenderSemaphore[cameraIndex]}, RenderFence[cameraIndex]);
  }
  Graphics::Vulkan::Swapchain::PresentImage(DisplaySurface.Swapchain, swapchainImageIndex, device.Queues.Present, RenderSemaphore);
  for (const auto mesh : meshes)
  {
    mesh->IsIndicesDirty = false;
    mesh->IsVerticesDirty = false;
  }
}
void Rendering::MeshView::Setup(Graphics::Vulkan::Device::Device device)
{
  this->DeviceInUse = device;
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
      this->StagingVertexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, verticesSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      this->VertexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    }
    Graphics::Vulkan::Buffer::SetData(this->StagingVertexBuffer, mesh->Vertices.data(), verticesSize);
    IsUpdated = true;
  }

  if (this->StagingIndexBuffer.Buffer == VK_NULL_HANDLE || mesh->IsIndicesDirty)
  {
    if (this->StagingIndexBuffer.Buffer == VK_NULL_HANDLE || this->StagingIndexBuffer.Size != indicesSize)
    {
      Graphics::Vulkan::Buffer::Destroy(this->StagingIndexBuffer);
      Graphics::Vulkan::Buffer::Destroy(this->IndexBuffer);
      this->StagingIndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, indicesSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      this->IndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    }
    Graphics::Vulkan::Buffer::SetData(this->StagingIndexBuffer, mesh->Indices.data(), indicesSize);
    this->IndexCount = mesh->Indices.size();
    IsUpdated = true;
  }

  if (!IsUpdated)
    return;

  if (this->GraphicsCommandPool.CommandPool == VK_NULL_HANDLE)
    this->GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  if (this->RenderCommand.Command == VK_NULL_HANDLE)
    this->RenderCommand = Graphics::Vulkan::Command::Create(device, this->GraphicsCommandPool, Graphics::Vulkan::Command::SECONDARY);
  if (this->TransferCommandPool.CommandPool == VK_NULL_HANDLE)
    this->TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
  if (this->TransferCommand.Command == VK_NULL_HANDLE)
  {
    this->TransferCommand = Graphics::Vulkan::Command::Create(device, this->TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
    Graphics::Vulkan::Command::Begin(this->TransferCommand, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
    Graphics::Vulkan::Command::CopyBuffer(this->TransferCommand, this->StagingVertexBuffer, this->VertexBuffer);
    Graphics::Vulkan::Command::CopyBuffer(this->TransferCommand, this->StagingIndexBuffer, this->IndexBuffer);
    Graphics::Vulkan::Command::End(this->TransferCommand);
  }
}
void Rendering::MeshView::OnDestroy()
{
  Graphics::Vulkan::Buffer::Destroy(StagingVertexBuffer);
  Graphics::Vulkan::Buffer::Destroy(VertexBuffer);
  Graphics::Vulkan::Buffer::Destroy(StagingIndexBuffer);
  Graphics::Vulkan::Buffer::Destroy(IndexBuffer);
  Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
}
} // namespace Systems
} // namespace Tortuga