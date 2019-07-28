#include "Screen.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Screen
{
Screen::Screen()
{
  this->Vulkan = Vulkan::Instance::Create();
  this->Windows.clear();
}
Screen::~Screen()
{
  for (uint32_t i = 0; i < this->Windows.size(); i++)
  {
    const auto window = this->Windows[i];
    window.ExitSignal->set_value();
    window.RenderThread->join();
    delete window.ExitSignal;
    delete window.RenderThread;
    Vulkan::Device::WaitForDevice(window.Device);
    Vulkan::CommandPool::Destroy(window.BufferTransferPool);
    Vulkan::CommandPool::Destroy(window.CommandPool);
    Vulkan::Renderer::Destroy(window.Renderer);
    Vulkan::Buffer::Destroy(window.Buffer);
    Vulkan::Buffer::Destroy(window.StagingBuffer);
    Vulkan::DescriptorPool::Destroy(window.DescriptorPool);
    Vulkan::DescriptorLayout::Destroy(window.DescriptorLayout);
    Vulkan::Swapchain::Destroy(window.Swapchain);
    Vulkan::Window::Destroy(window.Window);
  }

  this->Windows.clear();
  Vulkan::Instance::Destroy(this->Vulkan);
}
Screen _renderer = Screen();

FullWindow CreateWindow(const char *title, uint32_t width, uint32_t height)
{
  const auto device = _renderer.Vulkan.Devices[0];
  const auto window = Vulkan::Window::Create(_renderer.Vulkan, title, width, height);
  const auto swapchain = Vulkan::Swapchain::Create(device, window);
  std::vector<Graphics::Vulkan::DescriptorLayout::Binding> bindings(1);
  {
    bindings[0].DescriptorCount = 1;
    bindings[0].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
    bindings[0].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  }
  auto descriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(device, {bindings});
  auto descriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, descriptorLayout);
  auto descriptorSet = Graphics::Vulkan::DescriptorSets::Create(device, descriptorPool, {descriptorLayout});

  auto stagingBuffer = Graphics::Vulkan::Buffer::CreateHostSrc(device, sizeof(uint32_t));
  auto buffer = Graphics::Vulkan::Buffer::CreateDeviceOnlyDest(device, sizeof(uint32_t));
  Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(descriptorSet, 0, {buffer});

  auto renderer = Graphics::Vulkan::Renderer::Create(_renderer.Vulkan, device, width, height, {descriptorLayout});
  Graphics::Vulkan::Renderer::BindDescriptorSets(renderer, {descriptorSet});

  const auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
  const auto command = Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::PRIMARY);

  const auto bufferTransferPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
  const auto bufferTransfer = Graphics::Vulkan::Command::Create(device, bufferTransferPool, Graphics::Vulkan::Command::PRIMARY);
  Graphics::Vulkan::Command::Begin(bufferTransfer, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
  Graphics::Vulkan::Command::CopyBuffer(bufferTransfer, stagingBuffer, buffer);
  Graphics::Vulkan::Command::End(bufferTransfer);

  const auto exitSignal = new std::promise<void>();
  const auto renderThread = new std::thread(Graphics::Vulkan::Renderer::RenderThread, device, renderer, swapchain, command, std::move((*exitSignal).get_future()));
  FullWindow fullWindow = {device, window, swapchain, descriptorLayout, descriptorPool, stagingBuffer, buffer, commandPool, bufferTransferPool, bufferTransfer, renderer, exitSignal, renderThread};
  _renderer.Windows.push_back(fullWindow);
  return fullWindow;
}

void DestroyWindow(FullWindow window)
{
  for (uint32_t i = 0; i < _renderer.Windows.size(); i++)
  {
    if (_renderer.Windows[i].Window.Window == window.Window.Window)
    {
      auto window = _renderer.Windows[i];
      window.ExitSignal->set_value();
      window.RenderThread->join();
      delete window.ExitSignal;
      delete window.RenderThread;
      Vulkan::Device::WaitForDevice(window.Device);
      Vulkan::CommandPool::Destroy(window.CommandPool);
      Vulkan::CommandPool::Destroy(window.CommandPool);
      Vulkan::Renderer::Destroy(window.Renderer);
      Vulkan::Buffer::Destroy(window.Buffer);
      Vulkan::Buffer::Destroy(window.StagingBuffer);
      Vulkan::DescriptorPool::Destroy(window.DescriptorPool);
      Vulkan::DescriptorLayout::Destroy(window.DescriptorLayout);
      Vulkan::Swapchain::Destroy(window.Swapchain);
      Vulkan::Window::Destroy(window.Window);
      _renderer.Windows.erase(_renderer.Windows.begin() + i);
      break;
    }
  }
}
void UpdateRenderData(FullWindow window, std::vector<Mesh> data)
{
  uint32_t temp;
  Vulkan::Buffer::SetData(window.StagingBuffer, &temp, sizeof(uint32_t));
  Vulkan::Command::Submit({window.BufferTransferCommand}, window.Device.Queues.Transfer[1]);
  Vulkan::Device::WaitForQueue(window.Device.Queues.Transfer[1]);
}
SDL_Event PollEvents(FullWindow window)
{
  return Vulkan::Window::PollEvents(window.Window);
}
} // namespace Screen
} // namespace Graphics
} // namespace Tortuga