#include "./Tortuga.hpp"

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
  auto descriptorBindings = Graphics::Vulkan::DescriptorLayout::Binding();
  {
    descriptorBindings.DescriptorCount = 1;
    descriptorBindings.ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
    descriptorBindings.Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  }
  auto descriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(device, {descriptorBindings});
  auto descriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, descriptorLayout);
  auto descriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, descriptorLayout, descriptorPool);

  //Pipeline
  auto shader = Graphics::Vulkan::Shader::Create(device, Utils::IO::GetFileContents("Build/Shaders/ray-marching.comp"));
  auto pipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, {descriptorLayout}, shader);

  while (!window.SignalClose)
  {
    Graphics::Vulkan::Window::PollEvents(window);
  }

  //Pipeline
  Graphics::Vulkan::Pipeline::DestroyPipeline(pipeline);
  Graphics::Vulkan::Shader::Destroy(shader);

  //Descriptors
  Graphics::Vulkan::DescriptorSet::Destroy(descriptorSet);
  Graphics::Vulkan::DescriptorPool::Destroy(descriptorPool);
  Graphics::Vulkan::DescriptorLayout::Destroy(descriptorLayout);

  Graphics::Vulkan::Swapchain::Destroy(swapchain);
  Graphics::Vulkan::Window::Destroy(window);
  Graphics::Vulkan::Instance::Destroy(vulkan);
  return 0;
}