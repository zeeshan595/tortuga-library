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
  //geometry
  Graphics::Vulkan::Shader::Shader GeometryShader;
  Graphics::Vulkan::Pipeline::Pipeline GeometryPipeline;
  Graphics::Vulkan::Semaphore::Semaphore GeometrySemaphore;

  //Rendering (Ray marching)
  Graphics::Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
  Graphics::Vulkan::DescriptorSets::DescriptorSets DescriptorSets;

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

    //todo: start ray marching renderer pipeline
    //todo: make sure correct swapchain is bound
    //todo: present the image
  }

  Rendering()
  {
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
    //Descriptor Set
    {
      
    }

    //todo: create meshes buffer (copy all meshes to be rendered in a single buffer array)
    //todo: create image object where rendered image will be stored
    //todo: create ray marching pipeline
  }

  ~Rendering()
  {
    //geometry pipeline
    {
      Graphics::Vulkan::Pipeline::DestroyPipeline(GeometryPipeline);
      Graphics::Vulkan::Shader::Destroy(GeometryShader);
      Graphics::Vulkan::Semaphore::Destroy(GeometrySemaphore);
    }
  }
};
} // namespace Systems
} // namespace Tortuga

#endif