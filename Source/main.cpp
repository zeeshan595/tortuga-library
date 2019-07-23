#include "./Tortuga.hpp"
#include <shaderc/shaderc.h>

using namespace Tortuga;

int main()
{
  auto vulkan = Graphics::Vulkan::Instance::Create();

  for (uint32_t i = 0; i < vulkan.Devices.size(); i++)
    Console::Info("{0}: {1}: {2}", Console::Arguments() << i << vulkan.Devices[i].Score << vulkan.Devices[i].Properties.deviceName);

  auto device = vulkan.Devices[0];
  auto window = Graphics::Vulkan::Window::Create(vulkan, "Hello World", 1024, 768);
  auto swapchain = Graphics::Vulkan::Swapchain::Create(device, window);

  //Descriptors
  std::vector<Graphics::Vulkan::DescriptorLayout::Binding> bindings(1);
  {
    bindings[0] = {};
    bindings[0].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[0].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
    bindings[0].DescriptorCount = 1;
  }
  auto descriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(device, bindings);
  auto descriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, descriptorLayout);
  auto descriptorSet = Graphics::Vulkan::DescriptorSets::Create(device, descriptorLayout, descriptorPool);

  //Graphics::Vulkan::DescriptorSets::UpdateDescriptorSet(descriptorSet, 0, {});

  //Pipeline
  auto shaderCode = Utils::IO::GetFileContents("Shaders/ray-marching.comp");
  auto compiledShader = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::COMPUTE, shaderCode);
  auto shader = Graphics::Vulkan::Shader::Create(device, compiledShader);
  auto pipelineCache = Utils::IO::GetFileContents("Shaders/ray-marching.comp.cache");
  auto pipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, {descriptorLayout}, shader, pipelineCache);
  auto newPipelineCache = Graphics::Vulkan::Pipeline::GetCacheData(pipeline);
  if (pipelineCache != newPipelineCache)
    Utils::IO::SetFileContents("Shaders/ray-marching.comp.cache", newPipelineCache);

  while (!window.SignalClose)
  {
    Graphics::Vulkan::Window::PollEvents(window);
  }

  //Pipeline
  Graphics::Vulkan::Pipeline::DestroyPipeline(pipeline);
  Graphics::Vulkan::Shader::Destroy(shader);

  //Descriptors
  Graphics::Vulkan::DescriptorPool::Destroy(descriptorPool);
  Graphics::Vulkan::DescriptorLayout::Destroy(descriptorLayout);

  Graphics::Vulkan::Swapchain::Destroy(swapchain);
  Graphics::Vulkan::Window::Destroy(window);
  Graphics::Vulkan::Instance::Destroy(vulkan);
  return 0;
}