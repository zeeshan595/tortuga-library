#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include <future>
#include <thread>
#include <vulkan/vulkan.h>

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
  Graphics::Vulkan::Fence::Fence PresentWaiter;

  //geometry
  Graphics::Vulkan::Shader::Shader GeometryShader;
  Graphics::Vulkan::Pipeline::Pipeline GeometryPipeline;
  Graphics::Vulkan::Semaphore::Semaphore GeometrySemaphore;

  //combine meshes into single buffer
  Graphics::Vulkan::Buffer::Buffer MeshCombineBuffer;
  Graphics::Vulkan::Command::Command MeshCombineCommand;
  Graphics::Vulkan::Semaphore::Semaphore MeshCombineSemaphore;

  //rendering
  VkExtent2D SwapchainExtent;
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
    RenderInfo renderInfo = {};
    {
      renderInfo.WindowWidth = render->SwapchainExtent.width;
      renderInfo.WindowHeight = render->SwapchainExtent.height;
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
    render->RenderingBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnly(Core::Engine::GetMainDevice(), sizeof(glm::vec4) * render->SwapchainExtent.width * render->SwapchainExtent.height);

    Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(render->OutRenderingDescriptorSet, {render->RenderingInfoBuffer, render->RenderingBuffer});

    if (render->RenderingImage.Image != VK_NULL_HANDLE)
      Graphics::Vulkan::Image::Destroy(render->RenderingImage);
    render->RenderingImage = Graphics::Vulkan::Image::Create(Core::Engine::GetMainDevice(), render->SwapchainExtent.width, render->SwapchainExtent.height);
  }

public:
  void Update()
  {
    Render();
    Present();
  }

  void Render()
  {
    auto swapchain = Core::Screen::GetSwapchain();
    auto device = Core::Engine::GetMainDevice();

    //wait for previous render to finish before starting a new render process
    if (!Graphics::Vulkan::Fence::IsFenceSignaled(RenderingWaiter))
      return;

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

      Graphics::Vulkan::Command::Submit(meshCommands, device.Queues.Compute[0], {}, meshSemaphore);
    }

    //combine meshes into single buffer
    {
      auto totalSize = meshBuffers.size() * Component::MESH_SIZE;
      if (MeshCombineBuffer.Buffer == VK_NULL_HANDLE || totalSize != MeshCombineBuffer.Size)
      {
        //buffer needs to be recreated
        if (MeshCombineBuffer.Buffer != VK_NULL_HANDLE)
          Graphics::Vulkan::Buffer::Destroy(MeshCombineBuffer);

        MeshCombineBuffer = Graphics::Vulkan::Buffer::CreateHostDest(device, totalSize);
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
      Graphics::Vulkan::Command::Submit({MeshCombineCommand}, device.Queues.Transfer[0], meshSemaphore, {MeshCombineSemaphore});
    }

    //rendering
    {
      //check if swapchain was resized
      if (SwapchainExtent.width != swapchain.Extent.width || SwapchainExtent.height != swapchain.Extent.height)
      {
        SwapchainExtent = swapchain.Extent;
        UpdateWindowSize(this);
      }

      //submit command
      auto windowWidth = (SwapchainExtent.width / 8) + 1;
      auto windowHeight = (SwapchainExtent.height / 8) + 1;
      Graphics::Vulkan::Command::Begin(RenderingCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
      Graphics::Vulkan::Command::BindPipeline(RenderingCommand, VK_PIPELINE_BIND_POINT_COMPUTE, RenderingPipeline, {InRenderingDescriptorSet, OutRenderingDescriptorSet});
      Graphics::Vulkan::Command::Compute(RenderingCommand, windowWidth, windowHeight, 1);
      Graphics::Vulkan::Command::TransferImageLayout(RenderingCommand, RenderingImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      Graphics::Vulkan::Command::BufferToImage(RenderingCommand, RenderingBuffer, RenderingImage, {0, 0}, {SwapchainExtent.width, SwapchainExtent.height});
      Graphics::Vulkan::Command::End(RenderingCommand);
      Graphics::Vulkan::Command::Submit({RenderingCommand}, Core::Engine::GetMainDevice().Queues.Compute[0], {MeshCombineSemaphore}, {RenderingSemaphore}, RenderingWaiter);
    }
  }

  void Present()
  {
    auto swapchain = Core::Screen::GetSwapchain();
    auto window = Core::Screen::GetWindow();
    auto device = Core::Engine::GetMainDevice();
    //VK_KHR_shared_presentable_image
    if (!Graphics::Vulkan::Fence::IsFenceSignaled(PresentWaiter))
      return;

    Graphics::Vulkan::Fence::ResetFences({PresentWaiter});

    auto index = Graphics::Vulkan::Swapchain::AquireNextImage(swapchain);
    auto presentImage = Graphics::Vulkan::Swapchain::GetImage(swapchain, index);

    {
      Graphics::Vulkan::Command::Begin(PresentCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
      Graphics::Vulkan::Command::TransferImageLayout(PresentCommand, RenderingImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
      Graphics::Vulkan::Command::TransferImageLayout(PresentCommand, presentImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      Graphics::Vulkan::Command::BlitImage(PresentCommand, RenderingImage, presentImage, {SwapchainExtent.width, SwapchainExtent.height}, {0, 0}, {0, 0});
      Graphics::Vulkan::Command::TransferImageLayout(PresentCommand, presentImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
      Graphics::Vulkan::Command::End(PresentCommand);
    }
    Graphics::Vulkan::Command::Submit({PresentCommand}, device.Queues.Graphics[0], {RenderingSemaphore}, {PresentSemaphore}, PresentWaiter);
    Graphics::Vulkan::Swapchain::PresentImage(swapchain, index, device.Queues.Present, {PresentSemaphore});
  }

  Rendering()
  {
    auto vulkan = Core::Engine::GetVulkan();
    auto device = Core::Engine::GetMainDevice();

    //general
    {
      TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
      ComputeCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
      GraphicsCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
      RenderingWaiter = Graphics::Vulkan::Fence::Create(device, true);
      PresentWaiter = Graphics::Vulkan::Fence::Create(device, true);
    }

    //geometry pipeline
    {
      //shader
      auto geometryShaderCode = Utils::IO::GetFileContents("Shaders/Geometry.comp");
      auto compiledGeometryShaderCode = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::COMPUTE, geometryShaderCode);
      GeometryShader = Graphics::Vulkan::Shader::Create(device, compiledGeometryShaderCode);

      //pipeline
      GeometryPipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, GeometryShader, {}, {Component::GetMeshDescriptorLayout()});
      GeometrySemaphore = Graphics::Vulkan::Semaphore::Create(device);
    }

    //combine meshes into single buffer
    {
      MeshCombineBuffer.Buffer = VK_NULL_HANDLE;
      MeshCombineCommand = Graphics::Vulkan::Command::Create(device, TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
      MeshCombineSemaphore = Graphics::Vulkan::Semaphore::Create(device);
    }

    //rendering
    {
      auto swapchain = Core::Screen::GetSwapchain();
      SwapchainExtent = swapchain.Extent;
      RenderingInfoBufferStaging = Graphics::Vulkan::Buffer::CreateHostSrc(device, sizeof(RenderInfo));
      RenderingInfoBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnlyDest(device, sizeof(RenderInfo));

      InRenderingDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(device, 1);
      OutRenderingDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(device, 2);
      RenderingDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {InRenderingDescriptorLayout, OutRenderingDescriptorLayout}, 2);
      InRenderingDescriptorSet = Graphics::Vulkan::DescriptorSets::Create(device, RenderingDescriptorPool, InRenderingDescriptorLayout);
      OutRenderingDescriptorSet = Graphics::Vulkan::DescriptorSets::Create(device, RenderingDescriptorPool, OutRenderingDescriptorLayout);
      RenderingCommand = Graphics::Vulkan::Command::Create(device, ComputeCommandPool, Graphics::Vulkan::Command::PRIMARY);

      auto shaderCode = Utils::IO::GetFileContents("Shaders/Rendering.comp");
      auto compiledShader = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::COMPUTE, shaderCode);
      RenderingShader = Graphics::Vulkan::Shader::Create(device, compiledShader);
      RenderingPipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, RenderingShader, {}, {InRenderingDescriptorLayout, OutRenderingDescriptorLayout});
      RenderingSemaphore = Graphics::Vulkan::Semaphore::Create(device);

      RenderingBuffer.Buffer = VK_NULL_HANDLE;
      RenderingImage.Image = VK_NULL_HANDLE;
      UpdateWindowSize(this);
    }

    //present
    {
      PresentCommand = Graphics::Vulkan::Command::Create(device, GraphicsCommandPool, Graphics::Vulkan::Command::PRIMARY);
      PresentSemaphore = Graphics::Vulkan::Semaphore::Create(device);
    }
  }

  ~Rendering()
  {
    auto device = Core::Engine::GetMainDevice();

    Graphics::Vulkan::Device::WaitForDevice(device);
    Graphics::Vulkan::Device::WaitForQueue(device.Queues.Compute[0]);
    Graphics::Vulkan::Device::WaitForQueue(device.Queues.Transfer[0]);
    Graphics::Vulkan::Device::WaitForQueue(device.Queues.Graphics[0]);
    //general
    {
      Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
      Graphics::Vulkan::CommandPool::Destroy(ComputeCommandPool);
      Graphics::Vulkan::CommandPool::Destroy(GraphicsCommandPool);
      Graphics::Vulkan::Fence::Destroy(RenderingWaiter);
      Graphics::Vulkan::Fence::Destroy(PresentWaiter);
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