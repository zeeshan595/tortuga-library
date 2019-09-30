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
  data.StagingVertexIndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->VertexIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  //texture
  data.StagingTextureBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->Textures.size() * sizeof(glm::vec2), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.StagingTextureIndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->TextureIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  //normals
  data.StagingNormalBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->Normals.size() * sizeof(glm::vec4), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
  data.StagingNormalIndexBuffer = Graphics::Vulkan::Buffer::CreateHost(device, mesh->NormalIndices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

  //copy data to staging buffers
  Graphics::Vulkan::Buffer::SetData(data.StagingVertexBuffer, mesh->Vertices.data(), data.StagingVertexBuffer.Size);
  Graphics::Vulkan::Buffer::SetData(data.StagingTextureBuffer, mesh->Textures.data(), data.StagingTextureBuffer.Size);
  Graphics::Vulkan::Buffer::SetData(data.StagingNormalBuffer, mesh->Normals.data(), data.StagingNormalBuffer.Size);
  Graphics::Vulkan::Buffer::SetData(data.StagingVertexIndexBuffer, mesh->VertexIndices.data(), data.StagingVertexIndexBuffer.Size);
  Graphics::Vulkan::Buffer::SetData(data.StagingTextureIndexBuffer, mesh->TextureIndices.data(), data.StagingTextureIndexBuffer.Size);
  Graphics::Vulkan::Buffer::SetData(data.StagingNormalIndexBuffer, mesh->NormalIndices.data(), data.StagingNormalIndexBuffer.Size);

  return data;
}
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
  {
    mesh->CleanUp();
    Core::Engine::RemoveComponent<MeshView>(mesh->Root);
  }

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
       VK_SHADER_STAGE_COMPUTE_BIT,
       VK_SHADER_STAGE_COMPUTE_BIT},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
       VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER});
}
void Rendering::ProcessMeshBuffers(Graphics::Vulkan::Device::Device device)
{
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

  std::vector<Graphics::Vulkan::Buffer::Buffer> VertexBuffers;
  std::vector<Graphics::Vulkan::Buffer::Buffer> TextureBuffers;
  std::vector<Graphics::Vulkan::Buffer::Buffer> NormalBuffers;
  std::vector<Graphics::Vulkan::Buffer::Buffer> VertexIndexBuffers;
  std::vector<Graphics::Vulkan::Buffer::Buffer> TextureIndexBuffers;
  std::vector<Graphics::Vulkan::Buffer::Buffer> NormalIndexBuffers;
  uint32_t vertexByteSize = 0;
  uint32_t textureByteSize = 0;
  uint32_t normalByteSize = 0;
  uint32_t vertexIndexByteSize = 0;
  uint32_t textureIndexByteSize = 0;
  uint32_t normalIndexByteSize = 0;

  for (const auto view : Core::Engine::GetComponents<MeshView>())
  {
    if (Core::Engine::GetComponent<Components::Mesh>(view->Root) != nullptr)
    {
      VertexBuffers.push_back(view->StagingVertexBuffer);
      TextureBuffers.push_back(view->StagingTextureBuffer);
      NormalBuffers.push_back(view->StagingNormalBuffer);
      VertexIndexBuffers.push_back(view->StagingVertexIndexBuffer);
      TextureIndexBuffers.push_back(view->StagingTextureIndexBuffer);
      NormalIndexBuffers.push_back(view->StagingNormalIndexBuffer);

      vertexByteSize += view->StagingVertexBuffer.Size;
      textureByteSize += view->StagingTextureBuffer.Size;
      normalByteSize += view->StagingNormalBuffer.Size;
      vertexIndexByteSize += view->StagingVertexIndexBuffer.Size;
      textureIndexByteSize += view->StagingTextureIndexBuffer.Size;
      normalIndexByteSize += view->StagingNormalIndexBuffer.Size;
    }
    else
    {
      //auto cleanup meshes not being used
      view->CleanUp();
      Core::Engine::RemoveComponent<MeshView>(view->Root);
    }
  }

  //update full buffers
  if (MeshVertexBuffer.Buffer == VK_NULL_HANDLE || MeshVertexBuffer.Size != vertexByteSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshVertexBuffer);
    MeshVertexBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, vertexByteSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
  if (MeshTextureBuffer.Buffer == VK_NULL_HANDLE || MeshTextureBuffer.Size != textureByteSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshTextureBuffer);
    MeshTextureBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, textureByteSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
  if (MeshNormalBuffer.Buffer == VK_NULL_HANDLE || MeshNormalBuffer.Size != normalByteSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshNormalBuffer);
    MeshNormalBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, normalByteSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
  if (MeshVertexIndexBuffer.Buffer == VK_NULL_HANDLE || MeshVertexIndexBuffer.Size != vertexIndexByteSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshVertexIndexBuffer);
    MeshVertexIndexBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, vertexIndexByteSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
  if (MeshTextureIndexBuffer.Buffer == VK_NULL_HANDLE || MeshTextureIndexBuffer.Size != textureIndexByteSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshTextureIndexBuffer);
    MeshTextureIndexBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, textureIndexByteSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
  if (MeshNormalIndexBuffer.Buffer == VK_NULL_HANDLE || MeshNormalIndexBuffer.Size != normalIndexByteSize)
  {
    Graphics::Vulkan::Buffer::Destroy(MeshNormalIndexBuffer);
    MeshNormalIndexBuffer = Graphics::Vulkan::Buffer::CreateDevice(device, normalIndexByteSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
  }
  //update descriptor set
  Graphics::Vulkan::DescriptorSet::UpdateDescriptorSet(MeshDescriptorSet, {MeshVertexBuffer, MeshTextureBuffer, MeshNormalBuffer, MeshVertexIndexBuffer, MeshTextureIndexBuffer, MeshNormalIndexBuffer});

  //setup mesh copy command
  vertexByteSize = 0;
  textureByteSize = 0;
  normalByteSize = 0;
  vertexIndexByteSize = 0;
  textureIndexByteSize = 0;
  normalIndexByteSize = 0;
  Graphics::Vulkan::Command::Begin(MeshCopyCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  for (const auto view : Core::Engine::GetComponents<MeshView>())
  {
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingVertexBuffer, MeshVertexBuffer, 0, vertexByteSize);
    vertexByteSize += view->StagingVertexBuffer.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingTextureBuffer, MeshTextureBuffer, 0, textureByteSize);
    textureByteSize += view->StagingTextureBuffer.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingNormalBuffer, MeshNormalBuffer, 0, normalByteSize);
    normalByteSize += view->StagingNormalBuffer.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingVertexIndexBuffer, MeshVertexIndexBuffer, 0, vertexIndexByteSize);
    vertexIndexByteSize += view->StagingVertexIndexBuffer.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingTextureIndexBuffer, MeshTextureIndexBuffer, 0, textureIndexByteSize);
    textureIndexByteSize += view->StagingTextureIndexBuffer.Size;
    Graphics::Vulkan::Command::CopyBuffer(MeshCopyCommand, view->StagingNormalIndexBuffer, MeshNormalIndexBuffer, 0, normalIndexByteSize);
    normalIndexByteSize += view->StagingNormalIndexBuffer.Size;
  }
  Graphics::Vulkan::Command::End(MeshCopyCommand);
  Graphics::Vulkan::Command::Submit({MeshCopyCommand}, device.Queues.Compute[0], {}, {MeshSemaphore});
}
} // namespace Systems
} // namespace Tortuga