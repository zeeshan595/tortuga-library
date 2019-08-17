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
  struct Pixel
  {
    float R;
    float G;
    float B;
    float A;
  };
  struct RenderInfo
  {
    int32_t WindowWidth;
    int32_t WindowHeight;
  };

  //general
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool ComputeCommandPool;

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
  Graphics::Vulkan::DescriptorSets::DescriptorSets RenderingDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer RenderingBuffer;
  Graphics::Vulkan::Buffer::Buffer RenderingInfoBufferStaging;
  Graphics::Vulkan::Buffer::Buffer RenderingInfoBuffer;

  void UpdateWindowSize()
  {
    RenderInfo renderInfo = {};
    {
      renderInfo.WindowWidth = RenderingWindowSize[0];
      renderInfo.WindowHeight = RenderingWindowSize[1];
    }
    Graphics::Vulkan::Buffer::SetData(RenderingInfoBufferStaging, &renderInfo, sizeof(renderInfo));
    auto renderInfoTransfer = Graphics::Vulkan::Command::Create(Core::Engine::GetMainDevice(), TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
    Graphics::Vulkan::Command::Begin(renderInfoTransfer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::Vulkan::Command::CopyBuffer(renderInfoTransfer, RenderingInfoBufferStaging, RenderingInfoBuffer);
    Graphics::Vulkan::Command::End(renderInfoTransfer);
    Graphics::Vulkan::Command::Submit({renderInfoTransfer}, Core::Engine::GetMainDevice().Queues.Transfer[0]);
    Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Transfer[0]);
  }

public:
  void Update()
  {
    //geometry processing
    std::vector<Graphics::Vulkan::Buffer::Buffer> meshBuffers;
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

        meshBuffers.push_back(mesh->Buffer);
        meshCommands.push_back(mesh->Command);
        meshThreads.push_back(std::async(std::launch::async, [mesh, geometryPipeline] {
          Graphics::Vulkan::Command::Begin(mesh->Command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
          Graphics::Vulkan::Command::CopyBuffer(mesh->Command, mesh->Staging, mesh->Buffer);
          Graphics::Vulkan::Command::BindPipeline(mesh->Command, VK_PIPELINE_BIND_POINT_COMPUTE, geometryPipeline, {mesh->DescriptorSets});
          uint32_t computeGroupSize = (mesh->BufferData.VerticesSize / 16) + 1;
          Graphics::Vulkan::Command::Compute(mesh->Command, computeGroupSize, computeGroupSize, 1);
          Graphics::Vulkan::Command::End(mesh->Command);
        }));
      }
      for (uint32_t i = 0; i < meshThreads.size(); i++)
        meshThreads[i].wait();

      Graphics::Vulkan::Command::Submit(meshCommands, Core::Engine::GetMainDevice().Queues.Compute[0], {}, {GeometrySemaphore});
    }

    //combine meshes into single buffer
    {
      auto totalSize = meshBuffers.size() * Component::MESH_SIZE;
      if (totalSize != MeshCombineBuffer.Size)
      {
        //buffer needs to be recreated
        Graphics::Vulkan::Buffer::Destroy(MeshCombineBuffer);
        MeshCombineBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnly(Core::Engine::GetMainDevice(), totalSize);
        Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(RenderingDescriptorSet, 0, {MeshCombineBuffer});
      }
      Graphics::Vulkan::Command::Begin(MeshCombineCommand, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
      uint32_t offset = 0;
      for (auto meshBuffer : meshBuffers)
      {
        Graphics::Vulkan::Command::CopyBuffer(MeshCombineCommand, meshBuffer, MeshCombineBuffer, 0, offset);
        offset += meshBuffer.Size;
      }
      Graphics::Vulkan::Command::End(MeshCombineCommand);
      Graphics::Vulkan::Command::Submit({MeshCombineCommand}, Core::Engine::GetMainDevice().Queues.Transfer[0], {GeometrySemaphore}, {MeshCombineSemaphore});
    }

    //rendering
    {
      auto windowSize = Core::Screen::GetWindowSize();
      if (RenderingWindowSize != windowSize)
      {
        RenderingWindowSize = windowSize;
        Graphics::Vulkan::Buffer::Destroy(RenderingBuffer);
        RenderingBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnlySrc(Core::Engine::GetMainDevice(), sizeof(Pixel) * RenderingWindowSize[0] * RenderingWindowSize[1]);
        UpdateWindowSize();
      }
    }
    //todo: start ray marching renderer pipeline
    //todo: make sure correct swapchain is bound
    //todo: present the image
  }

  Rendering()
  {
    //general
    {
      TransferCommandPool = Graphics::Vulkan::CommandPool::Create(Core::Engine::GetMainDevice(), Core::Engine::GetMainDevice().QueueFamilies.Transfer.Index);
      ComputeCommandPool = Graphics::Vulkan::CommandPool::Create(Core::Engine::GetMainDevice(), Core::Engine::GetMainDevice().QueueFamilies.Compute.Index);
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
      MeshCombineBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnlyDest(Core::Engine::GetMainDevice(), 1);
      MeshCombineCommand = Graphics::Vulkan::Command::Create(Core::Engine::GetMainDevice(), TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);
      MeshCombineSemaphore = Graphics::Vulkan::Semaphore::Create(Core::Engine::GetMainDevice());
    }

    //rendering
    {
      RenderingWindowSize = Core::Screen::GetWindowSize();
      RenderingBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnlySrc(Core::Engine::GetMainDevice(), sizeof(Pixel) * RenderingWindowSize[0] * RenderingWindowSize[1]);
      RenderingInfoBufferStaging = Graphics::Vulkan::Buffer::CreateHostSrc(Core::Engine::GetMainDevice(), sizeof(RenderInfo));
      RenderingInfoBuffer = Graphics::Vulkan::Buffer::CreateDeviceOnlyDest(Core::Engine::GetMainDevice(), sizeof(RenderInfo));
      UpdateWindowSize();

      InRenderingDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(Core::Engine::GetMainDevice(), 2);
      OutRenderingDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(Core::Engine::GetMainDevice(), 1);
      RenderingDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(Core::Engine::GetMainDevice(), {InRenderingDescriptorLayout, OutRenderingDescriptorLayout}, 2);
      RenderingDescriptorSet = Graphics::Vulkan::DescriptorSets::Create(Core::Engine::GetMainDevice(), RenderingDescriptorPool, {InRenderingDescriptorLayout, OutRenderingDescriptorLayout});
      Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(RenderingDescriptorSet, 0, {RenderingInfoBuffer, RenderingBuffer});
    }

    //todo: create ray marching pipeline
  }

  ~Rendering()
  {
    //general
    {
      Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
      Graphics::Vulkan::CommandPool::Destroy(ComputeCommandPool);
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
      Graphics::Vulkan::DescriptorPool::Destroy(RenderingDescriptorPool);
      Graphics::Vulkan::DescriptorLayout::Destroy(OutRenderingDescriptorLayout);
    }
  }
};
} // namespace Systems
} // namespace Tortuga

#endif