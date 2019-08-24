#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include <future>
#include <thread>

#include "../Graphics/Vulkan/DescriptorLayout.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSets.hpp"
#include "../Graphics/Vulkan/Pipeline.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/Vulkan/Window.hpp"
#include "../Graphics/Vulkan/Semaphore.hpp"
#include "../Graphics/Vulkan/Fence.hpp"

#include "../Core/System.hpp"
#include "../Core/Screen.hpp"
#include "../Core/Entity.hpp"
#include "../Console.hpp"

#include "../Components/Mesh.hpp"
#include "../Components/Transform.hpp"

namespace Tortuga
{
namespace Systems
{
class Rendering : public Core::System
{
private:
  struct RenderInfo
  {
    uint32_t WindowWidth;
    uint32_t WindowHeight;
  };

  //general
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool ComputeCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;
  Graphics::Vulkan::Fence::Fence RenderingWaiter;

  //geometry
  Graphics::Vulkan::Shader::Shader GeometryShader;
  Graphics::Vulkan::Pipeline::Pipeline GeometryPipeline;
  Graphics::Vulkan::Semaphore::Semaphore GeometrySemaphore;

  //combine meshes into single buffer
  Graphics::Vulkan::Buffer::Buffer MeshCombineBuffer;
  Graphics::Vulkan::Command::Command MeshCombineCommand;
  Graphics::Vulkan::Semaphore::Semaphore MeshCombineSemaphore;

  //rendering
  std::vector<int32_t> RenderingWindowSize;
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout OutRenderingDescriptorLayout;
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout InRenderingDescriptorLayout;
  Graphics::Vulkan::DescriptorPool::DescriptorPool RenderingDescriptorPool;
  Graphics::Vulkan::DescriptorSets::DescriptorSets InRenderingDescriptorSet;
  Graphics::Vulkan::DescriptorSets::DescriptorSets OutRenderingDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer RenderingBuffer;
  Graphics::Vulkan::Buffer::Buffer RenderingInfoBufferStaging;
  Graphics::Vulkan::Buffer::Buffer RenderingInfoBuffer;
  Graphics::Vulkan::Shader::Shader RenderingShader;
  Graphics::Vulkan::Pipeline::Pipeline RenderingPipeline;
  Graphics::Vulkan::Command::Command RenderingCommand;
  Graphics::Vulkan::Semaphore::Semaphore RenderingSemaphore;
  Graphics::Vulkan::Image::Image RenderingImage;

  //present
  Graphics::Vulkan::Command::Command PresentCommand;
  Graphics::Vulkan::Semaphore::Semaphore PresentSemaphore;

  static void UpdateWindowSize(Rendering *render)
  {
    Graphics::Vulkan::Device::WaitForDevice(Core::Engine::GetMainDevice());
    Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Compute[0]);
    Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Transfer[0]);
    Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Graphics[0]);
    RenderInfo renderInfo = {};
    {
      renderInfo.WindowWidth = render->RenderingWindowSize[0];
      renderInfo.WindowHeight = render->RenderingWindowSize[1];
    }
    Graphics::Vulkan::Buffer::SetData(render->RenderingInfoBufferStaging, &renderInfo, sizeof(renderInfo));
    auto renderInfoTransfer = Graphics::Vulkan::Command::Create(Core::Engine::GetMainDevice(), render->TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
    Graphics::Vulkan::Command::Begin(renderInfoTransfer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::Vulkan::Command::CopyBuffer(renderInfoTransfer, render->RenderingInfoBufferStaging, render->RenderingInfoBuffer);
    Graphics::Vulkan::Command::End(renderInfoTransfer);
    Graphics::Vulkan::Command::Submit({renderInfoTransfer}, Core::Engine::GetMainDevice().Queues.Transfer[0]);
    Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Transfer[0]);

    if (render->RenderingBuffer.Buffer != VK_NULL_HANDLE)
      Graphics::Vulkan::Buffer::Destroy(render->RenderingBuffer);
    render->RenderingBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnly(Core::Engine::GetMainDevice(), sizeof(glm::vec4) * render->RenderingWindowSize[0] * render->RenderingWindowSize[1]);

    Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(render->OutRenderingDescriptorSet, {render->RenderingInfoBuffer, render->RenderingBuffer});

    if (render->RenderingImage.Image != VK_NULL_HANDLE)
      Graphics::Vulkan::Image::Destroy(render->RenderingImage);
    render->RenderingImage = Graphics::Vulkan::Image::Create(Core::Engine::GetMainDevice(), render->RenderingWindowSize[0], render->RenderingWindowSize[1]);
  }

public:
  void Update()
  {
    //wait for previous render to finish before starting a new render process
    if (!Graphics::Vulkan::Fence::IsFenceSignaled(RenderingWaiter))
    {
      auto temp = Core::Screen::GetSwapchain();
      std::cout << std::endl;
      return;
    }

    Graphics::Vulkan::Fence::ResetFences({RenderingWaiter});
    //geometry processing
    std::vector<Graphics::Vulkan::Buffer::Buffer> meshBuffers;
    std::vector<Graphics::Vulkan::Semaphore::Semaphore> meshSemaphore;
    {
      auto geometryPipeline = this->GeometryPipeline;
      auto entities = Core::Entity::GetAllEntities();
      std::vector<std::future<void>> meshThreads(0);
      std::vector<Graphics::Vulkan::Command::Command> meshCommands;
      for (auto entity : entities)
      {
        auto mesh = entity->GetComponent<Component::Mesh>();
        if (mesh == nullptr)
          continue;

        auto transform = entity->GetComponent<Component::Transform>();
        if (transform)
          mesh->ApplyTransformation(transform->Position, transform->Rotation, transform->Scale);

        meshBuffers.push_back(mesh->Buffer);
        if (mesh->IsStatic && mesh->IsProcessedOnce)
          continue;
        meshCommands.push_back(mesh->Command);
        meshThreads.push_back(std::async(std::launch::async, [mesh, geometryPipeline] {
          Graphics::Vulkan::Buffer::SetData(mesh->Staging, &mesh->BufferData, Component::MESH_SIZE);
          Graphics::Vulkan::Command::Begin(mesh->Command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
          Graphics::Vulkan::Command::CopyBuffer(mesh->Command, mesh->Staging, mesh->Buffer);
          Graphics::Vulkan::Command::BindPipeline(mesh->Command, VK_PIPELINE_BIND_POINT_COMPUTE, geometryPipeline, {mesh->DescriptorSets});
          uint32_t computeGroupSize = (mesh->BufferData.VerticesSize / 16) + 1;
          Graphics::Vulkan::Command::Compute(mesh->Command, computeGroupSize, computeGroupSize, 1);
          Graphics::Vulkan::Command::End(mesh->Command);
        }));
        mesh->IsProcessedOnce = true;
      }
      for (uint32_t i = 0; i < meshThreads.size(); i++)
        meshThreads[i].wait();

      if (meshCommands.size() > 0)
        meshSemaphore.push_back(GeometrySemaphore);

      Graphics::Vulkan::Command::Submit(meshCommands, Core::Engine::GetMainDevice().Queues.Compute[0], {}, meshSemaphore);
    }

    //combine meshes into single buffer
    {
      auto totalSize = meshBuffers.size() * Component::MESH_SIZE;
      if (MeshCombineBuffer.Buffer == VK_NULL_HANDLE || totalSize != MeshCombineBuffer.Size)
      {
        //buffer needs to be recreated
        if (MeshCombineBuffer.Buffer != VK_NULL_HANDLE)
          Graphics::Vulkan::Buffer::Destroy(MeshCombineBuffer);

        MeshCombineBuffer = Graphics::Vulkan::Buffer::CreateHostDest(Core::Engine::GetMainDevice(), totalSize);
        Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(InRenderingDescriptorSet, {MeshCombineBuffer});
      }
      Graphics::Vulkan::Command::Begin(MeshCombineCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
      uint32_t offset = 0;
      for (auto meshBuffer : meshBuffers)
      {
        Graphics::Vulkan::Command::CopyBuffer(MeshCombineCommand, meshBuffer, MeshCombineBuffer, 0, offset);
        offset += meshBuffer.Size;
      }
      Graphics::Vulkan::Command::End(MeshCombineCommand);
      Graphics::Vulkan::Command::Submit({MeshCombineCommand}, Core::Engine::GetMainDevice().Queues.Transfer[0], meshSemaphore, {MeshCombineSemaphore});
    }

    //rendering
    {
      //check if window was resized
      auto windowSize = Core::Screen::GetWindowSize();
      if (RenderingWindowSize != windowSize)
      {
        RenderingWindowSize = windowSize;
        UpdateWindowSize(this);
      }

      //submit command
      auto windowWidth = (RenderingWindowSize[0] / 8) + 1;
      auto windowHeight = (RenderingWindowSize[1] / 8) + 1;
      Graphics::Vulkan::Command::Begin(RenderingCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
      Graphics::Vulkan::Command::BindPipeline(RenderingCommand, VK_PIPELINE_BIND_POINT_COMPUTE, RenderingPipeline, {InRenderingDescriptorSet, OutRenderingDescriptorSet});
      Graphics::Vulkan::Command::Compute(RenderingCommand, windowWidth, windowHeight, 1);
      Graphics::Vulkan::Command::TransferImageLayout(RenderingCommand, RenderingImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      Graphics::Vulkan::Command::BufferToImage(RenderingCommand, RenderingBuffer, RenderingImage, {0, 0}, {RenderingWindowSize[0], RenderingWindowSize[1]});
      Graphics::Vulkan::Command::End(RenderingCommand);
      Graphics::Vulkan::Command::Submit({RenderingCommand}, Core::Engine::GetMainDevice().Queues.Compute[0], {MeshCombineSemaphore}, {RenderingSemaphore});
    }

    //present
    {
      auto index = Graphics::Vulkan::Swapchain::AquireNextImage(Core::Screen::GetSwapchain());
      auto presentImage = Graphics::Vulkan::Swapchain::GetImage(Core::Screen::GetSwapchain(), index);
      if (presentImage.Width != RenderingImage.Width || presentImage.Height != RenderingImage.Height)
        return;

      Graphics::Vulkan::Command::Begin(PresentCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
      Graphics::Vulkan::Command::TransferImageLayout(PresentCommand, RenderingImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
      Graphics::Vulkan::Command::TransferImageLayout(PresentCommand, presentImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      Graphics::Vulkan::Command::BlitImage(PresentCommand, RenderingImage, presentImage, {RenderingWindowSize[0], RenderingWindowSize[1]}, {0, 0}, {0, 0});
      Graphics::Vulkan::Command::TransferImageLayout(PresentCommand, presentImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
      Graphics::Vulkan::Command::End(PresentCommand);
      Graphics::Vulkan::Command::Submit({PresentCommand}, Core::Engine::GetMainDevice().Queues.Graphics[0], {RenderingSemaphore}, {PresentSemaphore}, RenderingWaiter);

      Graphics::Vulkan::Swapchain::PresentImage(Core::Screen::GetSwapchain(), index, Core::Engine::GetMainDevice().Queues.Present, {PresentSemaphore});
    }
  }

  Rendering()
  {
    //general
    {
      TransferCommandPool = Graphics::Vulkan::CommandPool::Create(Core::Engine::GetMainDevice(), Core::Engine::GetMainDevice().QueueFamilies.Transfer.Index);
      ComputeCommandPool = Graphics::Vulkan::CommandPool::Create(Core::Engine::GetMainDevice(), Core::Engine::GetMainDevice().QueueFamilies.Compute.Index);
      GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(Core::Engine::GetMainDevice(), Core::Engine::GetMainDevice().QueueFamilies.Graphics.Index);
      RenderingWaiter = Graphics::Vulkan::Fence::Create(Core::Engine::GetMainDevice(), true);
    }

    //geometry pipeline
    {
      //shader
      auto geometryShaderCode = Utils::IO::GetFileContents("Shaders/Geometry.comp");
      auto compiledGeometryShaderCode = Graphics::Vulkan::Shader::CompileShader(Core::Engine::GetVulkan(), Graphics::Vulkan::Shader::COMPUTE, geometryShaderCode);
      GeometryShader = Graphics::Vulkan::Shader::Create(Core::Engine::GetMainDevice(), compiledGeometryShaderCode);

      //pipeline
      GeometryPipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(Core::Engine::GetMainDevice(), GeometryShader, {}, {Component::GetMeshDescriptorLayout()});
      GeometrySemaphore = Graphics::Vulkan::Semaphore::Create(Core::Engine::GetMainDevice());
    }

    //combine meshes into single buffer
    {
      MeshCombineBuffer.Buffer = VK_NULL_HANDLE;
      MeshCombineCommand = Graphics::Vulkan::Command::Create(Core::Engine::GetMainDevice(), TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
      MeshCombineSemaphore = Graphics::Vulkan::Semaphore::Create(Core::Engine::GetMainDevice());
    }

    //rendering
    {
      RenderingWindowSize = Core::Screen::GetWindowSize();
      RenderingInfoBufferStaging = Graphics::Vulkan::Buffer::CreateHostSrc(Core::Engine::GetMainDevice(), sizeof(RenderInfo));
      RenderingInfoBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnlyDest(Core::Engine::GetMainDevice(), sizeof(RenderInfo));

      InRenderingDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(Core::Engine::GetMainDevice(), 1);
      OutRenderingDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(Core::Engine::GetMainDevice(), 2);
      RenderingDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(Core::Engine::GetMainDevice(), {InRenderingDescriptorLayout, OutRenderingDescriptorLayout}, 2);
      InRenderingDescriptorSet = Graphics::Vulkan::DescriptorSets::Create(Core::Engine::GetMainDevice(), RenderingDescriptorPool, InRenderingDescriptorLayout);
      OutRenderingDescriptorSet = Graphics::Vulkan::DescriptorSets::Create(Core::Engine::GetMainDevice(), RenderingDescriptorPool, OutRenderingDescriptorLayout);
      RenderingCommand = Graphics::Vulkan::Command::Create(Core::Engine::GetMainDevice(), ComputeCommandPool, Graphics::Vulkan::Command::PRIMARY);

      auto shaderCode = Utils::IO::GetFileContents("Shaders/Rendering.comp");
      auto compiledShader = Graphics::Vulkan::Shader::CompileShader(Core::Engine::GetVulkan(), Graphics::Vulkan::Shader::COMPUTE, shaderCode);
      RenderingShader = Graphics::Vulkan::Shader::Create(Core::Engine::GetMainDevice(), compiledShader);
      RenderingPipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(Core::Engine::GetMainDevice(), RenderingShader, {}, {InRenderingDescriptorLayout, OutRenderingDescriptorLayout});
      RenderingSemaphore = Graphics::Vulkan::Semaphore::Create(Core::Engine::GetMainDevice());

      RenderingBuffer.Buffer = VK_NULL_HANDLE;
      RenderingImage.Image = VK_NULL_HANDLE;
      UpdateWindowSize(this);
    }

    //present
    {
      PresentCommand = Graphics::Vulkan::Command::Create(Core::Engine::GetMainDevice(), GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);
      PresentSemaphore = Graphics::Vulkan::Semaphore::Create(Core::Engine::GetMainDevice());
    }
  }

  ~Rendering()
  {
    Graphics::Vulkan::Device::WaitForDevice(Core::Engine::GetMainDevice());
    Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Compute[0]);
    Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Transfer[0]);
    Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Graphics[0]);
    //general
    {
      Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
      Graphics::Vulkan::CommandPool::Destroy(ComputeCommandPool);
      Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
      Graphics::Vulkan::Fence::Destroy(RenderingWaiter);
    }

    //geometry pipeline
    {
      Graphics::Vulkan::Pipeline::DestroyPipeline(GeometryPipeline);
      Graphics::Vulkan::Shader::Destroy(GeometryShader);
      Graphics::Vulkan::Semaphore::Destroy(GeometrySemaphore);
    }

    //combine meshes into single buffer
    {
      Graphics::Vulkan::Buffer::Destroy(MeshCombineBuffer);
      Graphics::Vulkan::Semaphore::Destroy(MeshCombineSemaphore);
    }

    //rendering
    {
      Graphics::Vulkan::Buffer::Destroy(RenderingBuffer);
      Graphics::Vulkan::Buffer::Destroy(RenderingInfoBuffer);
      Graphics::Vulkan::Buffer::Destroy(RenderingInfoBufferStaging);
      Graphics::Vulkan::DescriptorLayout::Destroy(InRenderingDescriptorLayout);
      Graphics::Vulkan::DescriptorLayout::Destroy(OutRenderingDescriptorLayout);
      Graphics::Vulkan::DescriptorPool::Destroy(RenderingDescriptorPool);
      Graphics::Vulkan::Shader::Destroy(RenderingShader);
      Graphics::Vulkan::Pipeline::DestroyPipeline(RenderingPipeline);
      Graphics::Vulkan::Semaphore::Destroy(RenderingSemaphore);
      Graphics::Vulkan::Image::Destroy(RenderingImage);
    }

    //present
    {
      Graphics::Vulkan::Semaphore::Destroy(PresentSemaphore);
    }
  }
};
} // namespace Systems
} // namespace Tortuga

#endif