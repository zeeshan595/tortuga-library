#include "./Rendering.hpp"

namespace Tortuga
{
namespace Systems
{
struct CameraInfo
{
  glm::mat4 View;
  glm::mat4 Perspective;
  glm::vec2 ViewportOffset;
  glm::vec2 ViewportSize;

  CameraInfo()
  {
    View = glm::mat4(1.0);
    Perspective = glm::mat4(1.0);
    ViewportOffset = glm::vec2(0, 0);
    ViewportSize = glm::vec2(1, 1);
  }
};
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

  //record a sub-command for each mesh
  std::vector<Component::Mesh *> processedMeshes;
  std::vector<std::future<void>> vulkanThreads;
  std::vector<CameraInfo> cameraInfos;
  for (auto entity : Core::Entity::GetAllEntitiesWithComponent<Component::Camera>())
  {
    auto camera = entity->GetComponent<Component::Camera>();
    if (camera != nullptr)
    {
      CameraInfo cameraInfo = {};
      {
        cameraInfo.Perspective = glm::perspective(camera->FieldOfView, camera->AspectRatio, camera->NearClipPlane, camera->FarClipPlane);
        cameraInfo.Perspective[1][1] *= -1;
        cameraInfo.ViewportOffset = camera->ViewportOffset;
        cameraInfo.ViewportSize = camera->ViewportSize;
        auto transform = entity->GetComponent<Component::Transform>();
        if (transform)
          cameraInfo.View = transform->GetModelMatrix();
      }
      cameraInfos.push_back(cameraInfo);
    }
  }
  for (auto entity : Core::Entity::GetAllEntitiesWithComponent<Component::Mesh>())
  {
    auto mesh = entity->GetComponent<Component::Mesh>();
    if (mesh != nullptr)
    {
      //thread data
      const auto transform = entity->GetComponent<Component::Transform>();
      auto modelMatrix = glm::mat4(1.0);
      if (transform != nullptr)
        modelMatrix = transform->GetModelMatrix();

      const auto pipeline = Pipeline;
      const auto renderPass = RenderPass;
      const auto framebuffer = Framebuffers[swapchainIndex];
      const auto descriptorLayouts = DescriptorLayouts;
      //process mesh and make sure it is ready for render soon
      processedMeshes.push_back(mesh);
      vulkanThreads.push_back(std::async(std::launch::async, [mesh, pipeline, renderPass, framebuffer, device, descriptorLayouts, modelMatrix, cameraInfos] {
        if (mesh->RenderCommandPool.CommandPool == VK_NULL_HANDLE)
          mesh->RenderCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
        if (mesh->TransferCommandPool.CommandPool == VK_NULL_HANDLE)
          mesh->TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
        if (mesh->RenderCommand.Command == VK_NULL_HANDLE)
          mesh->RenderCommand = Graphics::Vulkan::Command::Create(device, mesh->RenderCommandPool, Graphics::Vulkan::Command::SECONDARY);
        if (mesh->TransferCommand.Command == VK_NULL_HANDLE)
          mesh->TransferCommand = Graphics::Vulkan::Command::Create(device, mesh->TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);

        //record mesh commands
        Graphics::Vulkan::Command::Begin(mesh->RenderCommand, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, renderPass, framebuffer);
        Graphics::Vulkan::Command::Begin(mesh->TransferCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

        //check if mesh vertices data is up to date
        uint vertexBufferSize = mesh->GetVerticesByteSize();
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
          Graphics::Vulkan::Buffer::SetData(mesh->StagingVertexBuffer, mesh->GetVertices().data(), vertexBufferSize);
          //data needs to be copied by transfer command
          Graphics::Vulkan::Command::CopyBuffer(mesh->TransferCommand, mesh->StagingVertexBuffer, mesh->VertexBuffer);
        }
        //check if mesh indices data is up to date
        uint32_t indexBufferSize = mesh->GetIndicesByteSize();
        if (mesh->StagingIndexBuffer.Buffer == VK_NULL_HANDLE || indexBufferSize != mesh->StagingIndexBuffer.Size)
        {
          //delete old buffers if they exist
          if (mesh->StagingIndexBuffer.Buffer != VK_NULL_HANDLE)
          {
            Graphics::Vulkan::Buffer::Destroy(mesh->StagingIndexBuffer);
            Graphics::Vulkan::Buffer::Destroy(mesh->IndexBuffer);
          }
          //create new buffers
          mesh->StagingIndexBuffer = Graphics::Vulkan::Buffer::Create(device, indexBufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
          mesh->IndexBuffer = Graphics::Vulkan::Buffer::Create(device, indexBufferSize, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
          //copy data
          Graphics::Vulkan::Buffer::SetData(mesh->StagingIndexBuffer, mesh->GetIndices().data(), indexBufferSize);
          //data needs to be copied by transfer command
          Graphics::Vulkan::Command::CopyBuffer(mesh->TransferCommand, mesh->StagingIndexBuffer, mesh->IndexBuffer);
        }
        //make sure mesh descriptor sets are initialized
        if (mesh->UniformBufferPool.size() != cameraInfos.size())
        {
          for (auto pool : mesh->UniformBufferPool)
            Graphics::Vulkan::DescriptorPool::Destroy(pool);
          for (auto buffer : mesh->StagingUniformBuffer)
            Graphics::Vulkan::Buffer::Destroy(buffer);
          for (auto buffer : mesh->UniformBuffer)
            Graphics::Vulkan::Buffer::Destroy(buffer);

          mesh->UniformBufferPool.clear();
          mesh->UniformBufferSets.clear();

          for (auto camera : cameraInfos)
          {
            auto descriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, descriptorLayouts);
            auto descriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, descriptorPool, descriptorLayouts[0]);
            mesh->UniformBufferPool.push_back(descriptorPool);
            mesh->UniformBufferSets.push_back(descriptorSet);

            //setup uniform buffer object
            mesh->StagingUniformBuffer.push_back(Graphics::Vulkan::Buffer::Create(device, sizeof(Graphics::UniformBufferObject), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_TRANSFER_SRC_BIT));
            auto uniformBuffer = Graphics::Vulkan::Buffer::Create(device, sizeof(Graphics::UniformBufferObject), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
            mesh->UniformBuffer.push_back(uniformBuffer);
            Graphics::Vulkan::DescriptorSet::UpdateDescriptorSets(descriptorSet, {uniformBuffer});
          }
        }
        for (uint32_t i = 0; i < cameraInfos.size(); i++)
        {
          auto camera = cameraInfos[i];
          //update ubo buffer
          Graphics::UniformBufferObject ubo = {};
          ubo.Projection = camera.Perspective;
          ubo.View = camera.View;
          ubo.Model = modelMatrix;

          Graphics::Vulkan::Buffer::SetData(mesh->StagingUniformBuffer[i], &ubo, sizeof(ubo));
          Graphics::Vulkan::Command::CopyBuffer(mesh->TransferCommand, mesh->StagingUniformBuffer[i], mesh->UniformBuffer[i]);

          Graphics::Vulkan::Command::BindPipeline(mesh->RenderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline, {mesh->UniformBufferSets[i]});
          Graphics::Vulkan::Command::SetViewport(
              mesh->RenderCommand,
              framebuffer.Width * camera.ViewportOffset.x, framebuffer.Height * camera.ViewportOffset.y,
              framebuffer.Width * camera.ViewportSize.x, framebuffer.Height * camera.ViewportSize.y);
          Graphics::Vulkan::Command::BindVertexBuffer(mesh->RenderCommand, {mesh->VertexBuffer});
          Graphics::Vulkan::Command::BindIndexBuffer(mesh->RenderCommand, mesh->IndexBuffer);
          Graphics::Vulkan::Command::DrawIndexed(mesh->RenderCommand, mesh->GetIndices().size());
        }
        Graphics::Vulkan::Command::End(mesh->TransferCommand);
        Graphics::Vulkan::Command::End(mesh->RenderCommand);
      }));
    }
  }
  //wait for all threads to complete
  for (uint32_t i = 0; i < vulkanThreads.size(); i++)
    vulkanThreads[i].wait();

  //launch present task in a new thread
  auto renderer = Renderer;
  auto renderSemaphore = RenderSemaphore;
  auto presentSemaphore = PresentSemaphore;
  auto renderFence = RenderFence;
  std::async(std::launch::async, [device, processedMeshes, renderer, swapchain, swapchainIndex, renderSemaphore, presentSemaphore, renderFence] {
    //transfer commands
    std::vector<Graphics::Vulkan::Command::Command> transferCommands(processedMeshes.size());
    for (uint32_t i = 0; i < processedMeshes.size(); i++)
      transferCommands[i] = processedMeshes[i]->TransferCommand;
    Graphics::Vulkan::Command::Submit(transferCommands, device.Queues.Transfer[0], {}, {renderSemaphore});

    //execute all sub-commands
    std::vector<Graphics::Vulkan::Command::Command> renderCommands(processedMeshes.size());
    for (uint32_t i = 0; i < processedMeshes.size(); i++)
      renderCommands[i] = processedMeshes[i]->RenderCommand;
    Graphics::Vulkan::Command::ExecuteCommands(renderer, renderCommands);
    Graphics::Vulkan::Command::EndRenderPass(renderer);
    Graphics::Vulkan::Command::End(renderer);

    //submit primary command
    Graphics::Vulkan::Command::Submit({renderer}, device.Queues.Graphics[0], {renderSemaphore}, {presentSemaphore}, renderFence);
    //present the image
    Graphics::Vulkan::Swapchain::PresentImage(swapchain, swapchainIndex, device.Queues.Graphics[0], {presentSemaphore});
  });
}

Rendering::Rendering()
{
  const auto vulkan = Core::Engine::GetVulkan();
  const auto device = Core::Engine::GetMainDevice();
  const auto swapchain = Core::Screen::GetSwapchain();

  //descriptor sets / uniform buffer object
  DescriptorLayouts.resize(1);
  {
    DescriptorLayouts[0] = Graphics::Vulkan::DescriptorLayout::Create(device, 1, VK_SHADER_STAGE_VERTEX_BIT);
  }

  //Command Pools
  GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);

  //shader
  auto vertexCode = Utils::IO::GetFileContents("Shaders/simple.vert");
  auto vertexCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::VERTEX, vertexCode);
  auto fragmentCode = Utils::IO::GetFileContents("Shaders/simple.frag");
  auto fragmentCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::FRAGMENT, fragmentCode);

  //pipeline & render pass
  VertexShader = Graphics::Vulkan::Shader::Create(device, vertexCompile);
  FragmentShader = Graphics::Vulkan::Shader::Create(device, fragmentCompile);
  RenderPass = Graphics::Vulkan::RenderPass::Create(device, swapchain.SurfaceFormat.format);
  Pipeline = Graphics::Vulkan::Pipeline::CreateGraphicsPipeline(
      device,
      VertexShader, FragmentShader,
      RenderPass,
      swapchain.Extent.width, swapchain.Extent.height,
      {Graphics::Vertex::getBindingDescription()},
      Graphics::Vertex::getAttributeDescriptions(),
      DescriptorLayouts);

  Framebuffers.resize(swapchain.Images.size());
  for (uint32_t i = 0; i < Framebuffers.size(); i++)
    Framebuffers[i] = Graphics::Vulkan::Framebuffer::Create(device, swapchain.Extent.width, swapchain.Extent.height, RenderPass, {swapchain.Views[i], swapchain.DepthImageView});

  Renderer = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);
  RenderFence = Graphics::Vulkan::Fence::Create(device, true);
  RenderSemaphore = Graphics::Vulkan::Semaphore::Create(device);
  PresentSemaphore = Graphics::Vulkan::Semaphore::Create(device);
}

Rendering::~Rendering()
{
  const auto vulkan = Core::Engine::GetVulkan();
  const auto device = Core::Engine::GetMainDevice();

  Graphics::Vulkan::Device::WaitForDevice(device);

  Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  for (auto layout : DescriptorLayouts)
    Graphics::Vulkan::DescriptorLayout::Destroy(layout);
  Graphics::Vulkan::Fence::Destroy(RenderFence);
  Graphics::Vulkan::Semaphore::Destroy(RenderSemaphore);
  Graphics::Vulkan::Semaphore::Destroy(PresentSemaphore);
  Graphics::Vulkan::Pipeline::Destroy(Pipeline);
  Graphics::Vulkan::RenderPass::Destroy(RenderPass);
  Graphics::Vulkan::Shader::Destroy(FragmentShader);
  Graphics::Vulkan::Shader::Destroy(VertexShader);
}
} // namespace Systems
} // namespace Tortuga
