#include "./Rendering.hpp"

namespace Tortuga
{
namespace Systems
{
void Rendering::Update()
{
  Core::Screen::Dispatch();
  //check if rendering is already in progress
  if (!Graphics::Vulkan::Fence::IsFenceSignaled(RenderFence))
    return;
  Graphics::Vulkan::Fence::ResetFences({RenderFence});

  const auto renderer = Renderer;
  const auto renderPass = RenderPass;
  const auto pipeline = Pipeline;
  const auto framebuffers = Framebuffers;
  const auto presentSemaphore = PresentSemaphore;
  const auto renderFence = RenderFence;
  const auto transferSemaphore = TransferSemaphore;
  const auto graphicsProcessSemaphore = GraphicsProcessSemaphore;

  const auto task = std::async(std::launch::async, [renderer, renderPass, framebuffers, transferSemaphore, graphicsProcessSemaphore, presentSemaphore, renderFence, pipeline] {
    const auto device = Core::Engine::GetMainDevice();
    const auto swapchain = Core::Screen::GetSwapchain();

    //get current swapchain image being rendered
    const auto swapchainIndex = Graphics::Vulkan::Swapchain::AquireNextImage(swapchain);

    //start recording primary command for gpu
    Graphics::Vulkan::Command::Begin(renderer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::Vulkan::Command::BeginRenderPass(renderer, renderPass, framebuffers[swapchainIndex], swapchain.Extent.width, swapchain.Extent.height);

    //record a sub-command for each mesh
    std::vector<Component::Mesh *> processedMeshes;
    std::vector<std::future<void>> vulkanThreads;
    //go through each mesh
    for (auto entity : Core::Entity::GetAllEntitiesWithComponent<Component::Mesh>())
    {
      const auto mesh = entity->GetComponent<Component::Mesh>();
      const auto material = entity->GetComponent<Component::Material>();
      const auto transform = entity->GetComponent<Component::Transform>();

      if (mesh != nullptr)
      {
        const auto framebuffer = framebuffers[swapchainIndex];
        //process mesh and make sure it is ready for render soon
        processedMeshes.push_back(mesh);
        vulkanThreads.push_back(std::async(std::launch::async, [mesh, material, transform, pipeline, renderPass, framebuffer] {
          Graphics::Vulkan::Command::Begin(mesh->RenderCommand, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, renderPass, 0, framebuffer);
          Graphics::Vulkan::Command::Begin(mesh->TransferCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
          Graphics::Vulkan::Command::Begin(mesh->GraphicsProcessCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

          if (material != nullptr)
            mesh->UpdateMaterial(material);
          mesh->UpdateBuffers(transform);

          const auto cameraEntities = Core::Entity::GetAllEntitiesWithComponent<Component::Camera>();
          for (const auto cameraEntity : cameraEntities)
          {
            const auto camera = cameraEntity->GetComponent<Component::Camera>();
            if (camera == nullptr)
              continue;
            const auto cameraTransform = cameraEntity->GetComponent<Component::Transform>();
            Graphics::Vulkan::Command::Begin(camera->TransferCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
            camera->UpdateBuffers(cameraTransform);
            Graphics::Vulkan::Command::End(camera->TransferCommand);

            const std::vector<Graphics::Vulkan::DescriptorSet::DescriptorSet> descriptorSets = {
                mesh->TransformDescriptorSet,
                camera->CameraDescriptorSet,
                mesh->LightDescriptorSet,
                mesh->AlbedoDescriptorSet};
            Graphics::Vulkan::Command::BindPipeline(mesh->RenderCommand, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline, descriptorSets);
            Graphics::Vulkan::Command::SetViewport(
                mesh->RenderCommand,
                framebuffer.Width * camera->GetViewportOffset().x, framebuffer.Height * camera->GetViewportOffset().y,
                framebuffer.Width * camera->GetViewportSize().x, framebuffer.Height * camera->GetViewportSize().y);
            Graphics::Vulkan::Command::BindVertexBuffer(mesh->RenderCommand, {mesh->VertexBuffer});
            Graphics::Vulkan::Command::BindIndexBuffer(mesh->RenderCommand, mesh->IndexBuffer);
            Graphics::Vulkan::Command::DrawIndexed(mesh->RenderCommand, mesh->GetIndices().size());
          }
          Graphics::Vulkan::Command::End(mesh->GraphicsProcessCommand);
          Graphics::Vulkan::Command::End(mesh->TransferCommand);
          Graphics::Vulkan::Command::End(mesh->RenderCommand);
        }));
      }
    }
    //wait for all threads to complete
    for (uint32_t i = 0; i < vulkanThreads.size(); i++)
      vulkanThreads[i].wait();

    //transfer commands
    std::vector<Graphics::Vulkan::Command::Command> transferCommands(processedMeshes.size());
    for (uint32_t i = 0; i < processedMeshes.size(); i++)
      transferCommands[i] = processedMeshes[i]->TransferCommand;
    for (const auto cameraEntity : Core::Entity::GetAllEntitiesWithComponent<Component::Camera>())
    {
      const auto camera = cameraEntity->GetComponent<Component::Camera>();
      if (camera == nullptr)
        continue;
      if (!camera->IsEnabled)
        continue;
      transferCommands.push_back(camera->TransferCommand);
    }
    Graphics::Vulkan::Command::Submit(transferCommands, device.Queues.Transfer[0], {}, {transferSemaphore});
    //graphics process commands
    std::vector<Graphics::Vulkan::Command::Command> graphicsProcessCommands(processedMeshes.size());
    for (uint32_t i = 0; i < processedMeshes.size(); i++)
      graphicsProcessCommands[i] = processedMeshes[i]->GraphicsProcessCommand;
    Graphics::Vulkan::Command::Submit(graphicsProcessCommands, device.Queues.Graphics[0], {}, {graphicsProcessSemaphore});

    //execute all sub-commands
    std::vector<Graphics::Vulkan::Command::Command> renderCommands(processedMeshes.size());
    for (uint32_t i = 0; i < processedMeshes.size(); i++)
      renderCommands[i] = processedMeshes[i]->RenderCommand;
    Graphics::Vulkan::Command::ExecuteCommands(renderer, renderCommands);
    Graphics::Vulkan::Command::EndRenderPass(renderer);
    Graphics::Vulkan::Command::End(renderer);

    //submit primary command
    Graphics::Vulkan::Command::Submit({renderer}, device.Queues.Graphics[0], {transferSemaphore, graphicsProcessSemaphore}, {presentSemaphore}, renderFence);
    //present the image
    Graphics::Vulkan::Swapchain::PresentImage(swapchain, swapchainIndex, device.Queues.Graphics[0], {presentSemaphore});
  });
  if (task.valid() == false)
  {
    Console::Error("Could not start rendering task");
  }
}

Rendering::Rendering()
{
  const auto vulkan = Core::Engine::GetVulkan();
  const auto device = Core::Engine::GetMainDevice();
  const auto swapchain = Core::Screen::GetSwapchain();

  //Command Pools
  TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
  GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);

  //shader
  auto vertexCode = Utils::IO::GetFileContents("Assets/Shaders/simple.vert");
  auto vertexCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::VERTEX, vertexCode);
  auto fragmentCode = Utils::IO::GetFileContents("Assets/Shaders/simple.frag");
  auto fragmentCompile = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::FRAGMENT, fragmentCode);

  //pipeline & render pass
  VertexShader = Graphics::Vulkan::Shader::Create(device, vertexCompile);
  FragmentShader = Graphics::Vulkan::Shader::Create(device, fragmentCompile);
  RenderPass = Graphics::Vulkan::RenderPass::Create(device, swapchain.SurfaceFormat.format);
  Pipeline = Graphics::Vulkan::Pipeline::CreateGraphicsPipeline(
      device,
      VertexShader, FragmentShader,
      RenderPass,
      {Graphics::Vertex::getBindingDescription()},
      Graphics::Vertex::getAttributeDescriptions(),
      {//Descriptor Layouts
       Core::Engine::GetVertexUniformBufferLayout(),
       Core::Engine::GetVertexUniformBufferLayout(),
       Core::Engine::GetVertexAndFragmentUniformBufferLayout(),
       Core::Engine::GetFragmentCombinedImageSampleLayout()});

  Framebuffers.resize(swapchain.Images.size());
  for (uint32_t i = 0; i < Framebuffers.size(); i++)
    Framebuffers[i] = Graphics::Vulkan::Framebuffer::Create(device, swapchain.Extent.width, swapchain.Extent.height, RenderPass, {swapchain.Views[i], swapchain.DepthImageView});

  Renderer = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);
  RenderFence = Graphics::Vulkan::Fence::Create(device, true);
  TransferSemaphore = Graphics::Vulkan::Semaphore::Create(device);
  GraphicsProcessSemaphore = Graphics::Vulkan::Semaphore::Create(device);
  PresentSemaphore = Graphics::Vulkan::Semaphore::Create(device);
}

Rendering::~Rendering()
{
  const auto vulkan = Core::Engine::GetVulkan();
  const auto device = Core::Engine::GetMainDevice();

  Graphics::Vulkan::Device::WaitForDevice(device);
  Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
  Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
  Graphics::Vulkan::Fence::Destroy(RenderFence);
  Graphics::Vulkan::Semaphore::Destroy(TransferSemaphore);
  Graphics::Vulkan::Semaphore::Destroy(GraphicsProcessSemaphore);
  Graphics::Vulkan::Semaphore::Destroy(PresentSemaphore);
  Graphics::Vulkan::Pipeline::Destroy(Pipeline);
  Graphics::Vulkan::RenderPass::Destroy(RenderPass);
  Graphics::Vulkan::Shader::Destroy(FragmentShader);
  Graphics::Vulkan::Shader::Destroy(VertexShader);
}
} // namespace Systems
} // namespace Tortuga
