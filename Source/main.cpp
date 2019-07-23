#include "./Tortuga.hpp"
#include <shaderc/shaderc.h>

struct Pixel
{
  float r;
  float g;
  float b;
};

const uint32_t WINDOW_WIDTH = 1024;
const uint32_t WINDOW_HEIGHT = 768;

using namespace Tortuga;

int main()
{
  auto vulkan = Graphics::Vulkan::Instance::Create();

  for (uint32_t i = 0; i < vulkan.Devices.size(); i++)
    Console::Info("{0}: {1}: {2}", Console::Arguments() << i << vulkan.Devices[i].Score << vulkan.Devices[i].Properties.deviceName);

  auto device = vulkan.Devices[0];
  auto window = Graphics::Vulkan::Window::Create(vulkan, "Hello World", WINDOW_WIDTH, WINDOW_HEIGHT);
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
  auto descriptorSets = Graphics::Vulkan::DescriptorSets::Create(device, descriptorPool, {descriptorLayout});

  //Pipeline
  auto shaderCode = Utils::IO::GetFileContents("Shaders/ray-marching.comp");
  auto compiledShader = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::COMPUTE, shaderCode);
  auto shader = Graphics::Vulkan::Shader::Create(device, compiledShader);
  auto pipelineCache = Utils::IO::GetFileContents("Shaders/ray-marching.comp.cache");
  auto pipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, shader, pipelineCache, {descriptorLayout});
  auto newPipelineCache = Graphics::Vulkan::Pipeline::GetCacheData(pipeline);
  if (pipelineCache != newPipelineCache)
    Utils::IO::SetFileContents("Shaders/ray-marching.comp.cache", newPipelineCache);

  uint32_t renderImageSize = sizeof(Pixel) * WINDOW_WIDTH * WINDOW_HEIGHT;
  auto buffer = Graphics::Vulkan::Buffer::Create(device, renderImageSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  Graphics::Vulkan::DescriptorSets::UpdateDescriptorSet(descriptorSets, 0, {buffer});

  auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
  auto command = Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::PRIMARY);

  Graphics::Vulkan::Command::Begin(command, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
  Graphics::Vulkan::Command::BindPipeline(command, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline, {descriptorSets});
  Graphics::Vulkan::Command::Compute(command, WINDOW_WIDTH / 16, WINDOW_HEIGHT / 16, 1);
  Graphics::Vulkan::Command::End(command);
  Graphics::Vulkan::Command::Submit({command}, device.Queues.Compute[0]);
  vkQueueWaitIdle(device.Queues.Compute[0]);
  std::vector<Pixel> render(renderImageSize);
  Graphics::Vulkan::Buffer::GetData(buffer, render.data(), renderImageSize);

  while (!window.SignalClose)
  {
    Graphics::Vulkan::Window::PollEvents(window);
  }
  vkQueueWaitIdle(device.Queues.Compute[0]);
  Graphics::Vulkan::CommandPool::Destroy(commandPool);
  Graphics::Vulkan::Buffer::Destroy(buffer);

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