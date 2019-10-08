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
  const auto device = VulkanInstance.Devices[0];

  if (!Graphics::Vulkan::Fence::IsFenceSignaled(PresentFence))
    return;
  Graphics::Vulkan::Fence::ResetFences({PresentFence});

  ProcessMeshBuffers(device);

  const auto meshSemaphore = MeshSemaphore;
  const auto renderCommand = RenderCommand;
  const auto renderPipeline = RenderPipeline;
  const auto meshDescriptorSet = MeshDescriptorSet;
  const auto renderDescriptorSet = RenderDescriptorSet;
  const auto renderingBuffer = RenderingBuffer;
  const auto renderImage = RenderedImage;
  const auto renderSemaphore = RenderSemaphore;

  const auto swapchain = DisplaySurface.Swapchain;
  const auto presentCommand = PresentCommand;
  const auto presentSemaphore = PresentSemaphore;
  const auto presentFence = PresentFence;
  //start rendering in another thread
  const auto task = std::async(
      std::launch::async, [device,
                           meshSemaphore,
                           renderCommand,
                           renderPipeline,
                           meshDescriptorSet,
                           renderDescriptorSet,
                           renderingBuffer,
                           renderImage,
                           renderSemaphore,
                           swapchain,
                           presentCommand,
                           presentSemaphore,
                           presentFence] {
        //render image
        Graphics::Vulkan::Command::Begin(renderCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        Graphics::Vulkan::Command::BindPipeline(renderCommand, VK_PIPELINE_BIND_POINT_COMPUTE, renderPipeline, {renderDescriptorSet, meshDescriptorSet});
        Graphics::Vulkan::Command::Compute(renderCommand, RENDER_WIDTH / 8, RENDER_HEIGHT / 8, 1);
        Graphics::Vulkan::Command::TransferImageLayout(renderCommand, renderImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Graphics::Vulkan::Command::BufferToImage(renderCommand, renderingBuffer, renderImage, {0, 0}, {renderImage.Width, renderImage.Height});
        Graphics::Vulkan::Command::TransferImageLayout(renderCommand, renderImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        Graphics::Vulkan::Command::End(renderCommand);
        Graphics::Vulkan::Command::Submit({renderCommand}, device.Queues.Compute[0], {meshSemaphore}, {renderSemaphore});
        Graphics::Vulkan::Device::WaitForQueue(device.Queues.Compute[0]);

        //present image
        const auto presentImageIndex = Graphics::Vulkan::Swapchain::AquireNextImage(swapchain);
        const auto presentImage = Graphics::Vulkan::Swapchain::GetImage(swapchain, presentImageIndex);
        Graphics::Vulkan::Command::Begin(presentCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        Graphics::Vulkan::Command::TransferImageLayout(presentCommand, presentImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Graphics::Vulkan::Command::BlitImage(presentCommand, renderImage, presentImage);
        Graphics::Vulkan::Command::TransferImageLayout(presentCommand, presentImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        Graphics::Vulkan::Command::End(presentCommand);
        Graphics::Vulkan::Command::Submit({presentCommand}, device.Queues.Graphics[0], {renderSemaphore}, {presentSemaphore}, presentFence);
        Graphics::Vulkan::Swapchain::PresentImage(swapchain, presentImageIndex, device.Queues.Present, {presentSemaphore});
      });
  if (!task.valid())
    Console::Error("failed to validate render task");
}
Rendering::Rendering()
{
  VulkanInstance = Graphics::Vulkan::Instance::Create();
  const auto device = VulkanInstance.Devices[0];
  DisplaySurface = Graphics::DisplaySurface::Create(VulkanInstance, device);
  GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  ComputeCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
  TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);

  SetupLayoutInformation(device);

  //shader code
  const auto shaderCode = Utils::IO::GetFileContents("Assets/Shaders/Renderer.comp");
  const auto compiledShaderCode = Graphics::Vulkan::Shader::CompileShader(VulkanInstance, Graphics::Vulkan::Shader::COMPUTE, shaderCode);
  RenderShader = Graphics::Vulkan::Shader::Create(device, compiledShaderCode);
  RenderPipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, RenderShader, {}, {RenderingDescriptorLayout, MeshDescriptorLayout});

  //rendering buffers
  RenderOptionsBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, sizeof(RenderOptions), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  RenderingBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, sizeof(glm::vec4) * RENDER_WIDTH * RENDER_HEIGHT, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
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
  RenderDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {RenderingDescriptorLayout});
  RenderDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, RenderDescriptorPool, RenderingDescriptorLayout);
  Graphics::Vulkan::DescriptorSet::UpdateDescriptorSet(RenderDescriptorSet, {RenderOptionsBuffer, RenderingBuffer});

  //rendered image
  RenderedImage = Graphics::Vulkan::Image::Create(device, RENDER_WIDTH, RENDER_HEIGHT, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);

  //Commands
  RenderCommand = Graphics::Vulkan::Command::Create(device, ComputeCommandPool, Graphics::Vulkan::Command::PRIMARY);
  PresentCommand = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);

  //sync
  RenderSemaphore = Graphics::Vulkan::Semaphore::Create(device);
  PresentSemaphore = Graphics::Vulkan::Semaphore::Create(device);
  PresentFence = Graphics::Vulkan::Fence::Create(device, true);

  //mesh descriptor
  MeshDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {MeshDescriptorLayout});
  MeshDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, MeshDescriptorPool, MeshDescriptorLayout);
  MeshCopyCommand = Graphics::Vulkan::Command::Create(device, TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
  MeshSemaphore = Graphics::Vulkan::Semaphore::Create(device);
}
Rendering::~Rendering()
{
  auto device = VulkanInstance.Devices[0];
  Graphics::Vulkan::Device::WaitForDevice(device);

  const auto meshes = Core::Engine::GetComponents<MeshView>();
  for (const auto mesh : meshes)
    Core::Engine::RemoveComponent<MeshView>(mesh->Root);

  Graphics::Vulkan::Buffer::Destroy(MeshBufferCounts);
  Graphics::Vulkan::Buffer::Destroy(MeshBufferData);
  Graphics::Vulkan::Buffer::Destroy(MeshBufferIndices);
  Graphics::Vulkan::Buffer::Destroy(MeshBufferFaces);
  Graphics::Vulkan::Buffer::Destroy(MeshBufferNodes);
  Graphics::Vulkan::DescriptorPool::Destroy(MeshDescriptorPool);

  Graphics::Vulkan::Semaphore::Destroy(MeshSemaphore);
  Graphics::Vulkan::Semaphore::Destroy(RenderSemaphore);
  Graphics::Vulkan::Semaphore::Destroy(PresentSemaphore);
  Graphics::Vulkan::Fence::Destroy(PresentFence);

  Graphics::Vulkan::DescriptorPool::Destroy(RenderDescriptorPool);
  Graphics::Vulkan::Buffer::Destroy(RenderOptionsBuffer);
  Graphics::Vulkan::Buffer::Destroy(RenderingBuffer);
  Graphics::Vulkan::Image::Destroy(RenderedImage);

  Graphics::Vulkan::Pipeline::Destroy(RenderPipeline);
  Graphics::Vulkan::Shader::Destroy(RenderShader);
  Graphics::Vulkan::DescriptorLayout::Destroy(RenderingDescriptorLayout);
  Graphics::Vulkan::DescriptorLayout::Destroy(MeshDescriptorLayout);

  Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  Graphics::Vulkan::CommandPool::Destroy(ComputeCommandPool);
  Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
  Graphics::DisplaySurface::Destroy(DisplaySurface);
  Graphics::Vulkan::Instance::Destroy(VulkanInstance);
}
void Rendering::SetupLayoutInformation(Graphics::Vulkan::Device::Device device)
{
  //layout information
  RenderingDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(
      device,
      {VK_SHADER_STAGE_COMPUTE_BIT, VK_SHADER_STAGE_COMPUTE_BIT},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER});
  MeshDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(
      device,
      {VK_SHADER_STAGE_COMPUTE_BIT,
       VK_SHADER_STAGE_COMPUTE_BIT,
       VK_SHADER_STAGE_COMPUTE_BIT,
       VK_SHADER_STAGE_COMPUTE_BIT,
       VK_SHADER_STAGE_COMPUTE_BIT},
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
       VK_DESCRIPTOR_TYPE_STORAGE_BUFFER});
}
void Rendering::ProcessMeshBuffers(Graphics::Vulkan::Device::Device device)
{
  const auto meshes = Core::Engine::GetComponents<Components::Mesh>();
  if (meshes.size() == 0)
    return;
  for (const auto mesh : meshes)
  {
    if (Core::Engine::GetComponent<MeshView>(mesh->Root) == nullptr)
    {
      MeshView viewData = {};
      viewData.DeviceInUse = device;
      Core::Engine::AddComponent<MeshView>(mesh->Root, viewData);
    }
  }

  uint32_t meshCountsSize = 0;
  uint32_t meshDataSize = 0;
  uint32_t meshIndicesSize = 0;
  uint32_t meshFacesSize = 0;
  uint32_t meshNodesSize = 0;
  for (const auto view : Core::Engine::GetComponents<MeshView>())
  {
    if (Core::Engine::GetComponent<Components::Mesh>(view->Root) == nullptr)
    {
      Core::Engine::RemoveComponent<Components::Mesh>(view->Root);
      continue;
    }

    meshCountsSize += view->StagingMeshCounts.Size;
    meshDataSize += view->StagingMeshData.Size;
    meshIndicesSize += view->StagingIndices.Size;
    meshFacesSize += view->StagingFaces.Size;
    meshNodesSize += view->StagingNodes.Size;
  }

  if (MeshBufferCounts.Buffer == VK_NULL_HANDLE || MeshBufferCounts.Size != meshCountsSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshBufferCounts);
    MeshBufferCounts = Graphics::Vulkan::Buffer::CreateDevice(device, meshCountsSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  }
  if (MeshBufferData.Buffer == VK_NULL_HANDLE || MeshBufferData.Size != meshDataSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshBufferData);
    MeshBufferData = Graphics::Vulkan::Buffer::CreateDevice(device, meshDataSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  }
  if (MeshBufferIndices.Buffer == VK_NULL_HANDLE || MeshBufferIndices.Size != meshIndicesSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshBufferIndices);
    MeshBufferIndices = Graphics::Vulkan::Buffer::CreateDevice(device, meshIndicesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  }
  if (MeshBufferFaces.Buffer == VK_NULL_HANDLE || MeshBufferFaces.Size != meshFacesSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshBufferFaces);
    MeshBufferFaces = Graphics::Vulkan::Buffer::CreateDevice(device, meshFacesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  }
  if (MeshBufferNodes.Buffer == VK_NULL_HANDLE || MeshBufferNodes.Size != meshNodesSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshBufferNodes);
    MeshBufferNodes = Graphics::Vulkan::Buffer::CreateDevice(device, meshNodesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  }

  uint32_t meshCountsOffset = 0;
  uint32_t meshDataOffset = 0;
  uint32_t meshIndicesOffset = 0;
  uint32_t meshFaceOffset = 0;
  uint32_t meshNodesOffset = 0;
  Graphics::Vulkan::Command::Begin(MeshCopyCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  for (const auto view : Core::Engine::GetComponents<MeshView>())
  {
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingMeshCounts, MeshBufferCounts, 0, meshCountsOffset);
    meshCountsOffset += view->StagingMeshCounts.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingMeshData, MeshBufferData, 0, meshDataOffset);
    meshDataOffset += view->StagingMeshData.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingIndices, MeshBufferIndices, 0, meshIndicesOffset);
    meshIndicesOffset += view->StagingIndices.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingFaces, MeshBufferFaces, 0, meshFaceOffset);
    meshFaceOffset += view->StagingFaces.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingNodes, MeshBufferNodes, 0, meshNodesOffset);
    meshNodesOffset += view->StagingNodes.Size;
  }
  Graphics::Vulkan::Command::End(MeshCopyCommand);
  Graphics::Vulkan::Command::Submit({MeshCopyCommand}, device.Queues.Transfer[0], {}, {MeshSemaphore});

  Graphics::Vulkan::DescriptorSet::UpdateDescriptorSet(MeshDescriptorSet, {MeshBufferCounts, MeshBufferData, MeshBufferIndices, MeshBufferFaces, MeshBufferNodes});
}
} // namespace Systems
} // namespace Tortuga