#include "./Rendering.hpp"

#define RENDER_WIDTH 1920
#define RENDER_HEIGHT 1080

namespace Tortuga
{
namespace Systems
{
Rendering::MeshView CreateMeshBuffers(Graphics::Vulkan::Device::Device device, const Components::Mesh *mesh)
{
  auto data = Rendering::MeshView();
  //vertex
  data.StagingVertexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->Vertices.size() * sizeof(glm::vec4), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.VertexBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, data.StagingVertexBuffer.Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.StagingVertexIndicesBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->VertexIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.VertexIndicesBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, data.StagingVertexIndicesBuffer.Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  //texture
  data.StagingTextureBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->Textures.size() * sizeof(glm::vec2), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.TextureBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, data.StagingTextureBuffer.Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.StagingTextureIndicesBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->TextureIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.TextureIndicesBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, data.StagingTextureIndicesBuffer.Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  //normals
  data.StagingNormalBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->Normals.size() * sizeof(glm::vec4), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.NormalBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, data.StagingNormalBuffer.Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.StagingNormalIndicesBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->NormalIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.NormalIndicesBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, data.StagingNormalIndicesBuffer.Size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

  return data;
}
void DestroyMeshBuffers(Rendering::MeshView *data)
{
  Graphics::Vulkan::Buffer::Destroy(data->StagingVertexBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->VertexBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->StagingVertexIndicesBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->VertexIndicesBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->StagingTextureBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->TextureBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->StagingTextureIndicesBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->TextureIndicesBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->StagingNormalBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->NormalBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->StagingNormalIndicesBuffer);
  Graphics::Vulkan::Buffer::Destroy(data->NormalIndicesBuffer);
}
void Rendering::Update()
{
  Graphics::DisplaySurface::Dispatch(DisplaySurface);
  const auto device = VulkanInstance.Devices[0];

  if (!Graphics::Vulkan::Fence::IsFenceSignaled(PresentFence))
    return;
  Graphics::Vulkan::Fence::ResetFences({PresentFence});

  //auto cleanup
  const auto meshViews = Core::Engine::GetComponents<MeshView>();
  for (const auto view : meshViews)
  {
    if (Core::Engine::GetComponent<Components::Mesh>(view->Root) == nullptr)
    {
      DestroyMeshBuffers(view);
      Core::Engine::RemoveComponent<MeshView>(view->Root);
    }
  }
  //generate mesh buffer objects for vulkan
  const auto meshes = Core::Engine::GetComponents<Components::Mesh>();
  for (const auto mesh : meshes)
  {
    auto meshView = Core::Engine::GetComponent<MeshView>(mesh->Root);
    if (meshView == nullptr)
    {
      const auto data = CreateMeshBuffers(device, mesh);
      Core::Engine::AddComponent<MeshView>(mesh->Root, data);
    }
  }

  const auto renderCommand = RenderCommand;
  const auto renderPipeline = RenderPipeline;
  const auto renderDescriptorSet = RenderDescriptorSet;
  const auto renderingBuffer = RenderingBuffer;
  const auto renderImage = RenderedImage;
  const auto renderSemaphore = RenderSemaphore;

  const auto swapchain = DisplaySurface.Swapchain;
  const auto presentCommand = PresentCommand;
  const auto presentSemaphore = PresentSemaphore;
  const auto presentFence = PresentFence;
  const auto task = std::async(
      std::launch::async, [device,
                           renderCommand,
                           renderPipeline,
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
        Graphics::Vulkan::Command::BindPipeline(renderCommand, VK_PIPELINE_BIND_POINT_COMPUTE, renderPipeline, {renderDescriptorSet});
        Graphics::Vulkan::Command::Compute(renderCommand, RENDER_WIDTH / 8, RENDER_HEIGHT / 8, 1);
        Graphics::Vulkan::Command::TransferImageLayout(renderCommand, renderImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        Graphics::Vulkan::Command::BufferToImage(renderCommand, renderingBuffer, renderImage, {0, 0}, {renderImage.Width, renderImage.Height});
        Graphics::Vulkan::Command::TransferImageLayout(renderCommand, renderImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        Graphics::Vulkan::Command::End(renderCommand);
        Graphics::Vulkan::Command::Submit({renderCommand}, device.Queues.Compute[0], {}, {renderSemaphore});
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
  {
    Console::Error("failed to validate render task");
  }
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
  DescriptorLayouts.push_back(Graphics::Vulkan::DescriptorLayout::Create(
      device,
      {VK_SHADER_STAGE_COMPUTE_BIT, VK_SHADER_STAGE_COMPUTE_BIT},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER}));

  //shader code
  const auto shaderCode = Utils::IO::GetFileContents("Assets/Shaders/Renderer.comp");
  const auto compiledShaderCode = Graphics::Vulkan::Shader::CompileShader(VulkanInstance, Graphics::Vulkan::Shader::COMPUTE, shaderCode);
  RenderShader = Graphics::Vulkan::Shader::Create(device, compiledShaderCode);
  RenderPipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, RenderShader, {}, DescriptorLayouts);

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
  RenderDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {DescriptorLayouts});
  RenderDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, RenderDescriptorPool, DescriptorLayouts[0]);
  Graphics::Vulkan::DescriptorSet::UpdateDescriptorSet(RenderDescriptorSet, {RenderOptionsBuffer, RenderingBuffer});

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
  auto device = VulkanInstance.Devices[0];
  Graphics::Vulkan::Device::WaitForDevice(device);
  const auto meshes = Core::Engine::GetComponents<MeshView>();
  for (const auto mesh : meshes)
  {
    DestroyMeshBuffers(mesh);
    Core::Engine::RemoveComponent<MeshView>(mesh->Root);
  }
  Graphics::Vulkan::Semaphore::Destroy(RenderSemaphore);
  Graphics::Vulkan::Semaphore::Destroy(PresentSemaphore);
  Graphics::Vulkan::Fence::Destroy(PresentFence);

  Graphics::Vulkan::DescriptorPool::Destroy(RenderDescriptorPool);
  Graphics::Vulkan::Buffer::Destroy(RenderOptionsBuffer);
  Graphics::Vulkan::Buffer::Destroy(RenderingBuffer);
  Graphics::Vulkan::Image::Destroy(RenderedImage);

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