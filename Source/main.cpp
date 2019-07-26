#include "./Tortuga.hpp"
#include <thread>
#include <future>

struct sphere
{
  float radius;
};

using namespace Tortuga;

int main()
{
  const auto vulkan = Graphics::Vulkan::Instance::Create();
  const auto device = vulkan.Devices[0];

  auto window = Graphics::Vulkan::Window::Create(vulkan, "Tortuga", 2560, 1440);
  auto swapchain = Graphics::Vulkan::Swapchain::Create(device, window);

  std::vector<Graphics::Vulkan::DescriptorLayout::Binding> bindings(1);
  {
    bindings[0].DescriptorCount = 1;
    bindings[0].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
    bindings[0].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  }
  auto descriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(device, {bindings});
  auto descriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, descriptorLayout);
  auto descriptorSet = Graphics::Vulkan::DescriptorSets::Create(device, descriptorPool, {descriptorLayout});

  auto buffer = Graphics::Vulkan::Buffer::Create(device, sizeof(uint32_t), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  uint32_t radius = 3.0f;
  Graphics::Vulkan::Buffer::SetData(buffer, &radius, sizeof(uint32_t));
  Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(descriptorSet, 0, {buffer});

  const auto renderer = Graphics::Renderer::Create(vulkan, device, window.Width, window.Height, {descriptorLayout});
  Graphics::Renderer::BindDescriptorSets(renderer, {descriptorSet});

  const auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  const auto command = Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::PRIMARY);

  std::promise<void> exitSignal;
  std::thread renderThread(Graphics::Renderer::RenderThread, device, renderer, swapchain, command, std::move(exitSignal.get_future()));

  while (!window.SignalClose)
  {
    Graphics::Vulkan::Window::PollEvents(window);
  }

  //Exit signal
  exitSignal.set_value();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  Graphics::Vulkan::Device::WaitForDevice(device);

  Graphics::Vulkan::Buffer::Destroy(buffer);

  Graphics::Vulkan::DescriptorPool::Destroy(descriptorPool);
  Graphics::Vulkan::DescriptorLayout::Destroy(descriptorLayout);

  Graphics::Vulkan::CommandPool::Destroy(commandPool);
  Graphics::Renderer::Destroy(renderer);
  Graphics::Vulkan::Swapchain::Destroy(swapchain);
  Graphics::Vulkan::Window::Destroy(window);
  Graphics::Vulkan::Instance::Destroy(vulkan);
}