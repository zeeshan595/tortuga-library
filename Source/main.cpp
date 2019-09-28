#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  auto vulkan = Graphics::Vulkan::Instance::Create();
  auto device = vulkan.Devices[0];
  auto display = Graphics::DisplaySurface::Create(vulkan, device);

  std::vector<Graphics::Vulkan::DescriptorLayout::DescriptorLayout> descriptorLayouts;
  descriptorLayouts.push_back(Graphics::Vulkan::DescriptorLayout::Create(device, 1, VK_SHADER_STAGE_COMPUTE_BIT, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER));
  descriptorLayouts.push_back(Graphics::Vulkan::DescriptorLayout::Create(device, 1, VK_SHADER_STAGE_COMPUTE_BIT, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER));
  
  auto shaderCode = Utils::IO::GetFileContents("Assets/Shaders/ray.comp");
  auto compiledShaderCode = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::COMPUTE, shaderCode);
  auto shader = Graphics::Vulkan::Shader::Create(device, compiledShaderCode);
  auto pipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, shader, {}, descriptorLayouts);

  auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
  auto command = Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::PRIMARY);

  while (!Graphics::DisplaySurface::ShouldClose(display)) {
    Graphics::DisplaySurface::Dispatch(display);
  }

  Graphics::DisplaySurface::Destroy(display);
  Graphics::Vulkan::Instance::Destroy(vulkan);
  return EXIT_SUCCESS;
}