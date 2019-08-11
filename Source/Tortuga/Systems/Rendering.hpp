#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include "../Graphics/Vulkan/DescriptorLayout.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSets.hpp"
#include "../Graphics/Vulkan/Pipeline.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/Vulkan/Window.hpp"

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
  Graphics::Vulkan::Shader::Shader GeometryShader;
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout GeometryDescriptorLayout;
  Graphics::Vulkan::Pipeline::Pipeline GeometryPipeline;

public:
  void Update()
  {
    //todo: run all meshes through geometry pipeline
    //todo: Wait for all transformations to finish
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

      //descriptor layout
      std::vector<Graphics::Vulkan::DescriptorLayout::Binding> bindings(2);
      {
        //input
        bindings[0].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[0].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
        bindings[0].DescriptorCount = 1;
        bindings[0].Sampler = VK_NULL_HANDLE;

        //output
        bindings[1].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[1].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
        bindings[1].DescriptorCount = 1;
        bindings[1].Sampler = VK_NULL_HANDLE;
      }
      GeometryDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(Core::Engine::GetMainDevice(), bindings);

      //pipeline
      GeometryPipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(Core::Engine::GetMainDevice(), GeometryShader, {}, {GeometryDescriptorLayout});
    }
    //todo: create geometry pipeline
    //todo: create image object where rendered image will be stored
    //todo: create ray marching pipeline
  }

  ~Rendering()
  {
    //geometry pipeline
    {
      Graphics::Vulkan::Pipeline::DestroyPipeline(GeometryPipeline);
      Graphics::Vulkan::DescriptorLayout::Destroy(GeometryDescriptorLayout);
      Graphics::Vulkan::Shader::Destroy(GeometryShader);
    }
  }
};
} // namespace Systems
} // namespace Tortuga

#endif