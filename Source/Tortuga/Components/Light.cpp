#include "./Light.hpp"

namespace Tortuga
{
namespace Component
{
Light::Light()
{
  IsEnabled = true;
  IsStatic = false;
  IsProcessed = false;
  Data.Position = glm::vec4(0, 0, 0, 1);
  Data.Rotation = glm::vec4(0, 0, 0, 1);
  Data.Type = LightType::Point;
  Data.Intensity = 1.0f;
  Data.Range = 10.0f;

  auto device = Core::Engine::GetMainDevice();

  Staging = Graphics::Vulkan::Buffer::CreateHostSrc(device, sizeof(LightData));
  Buffer = Graphics::Vulkan::Buffer::CreateDeviceOnly(device, sizeof(LightData));

  CommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
  Command = Graphics::Vulkan::Command::Create(device, CommandPool, Graphics::Vulkan::Command::PRIMARY);
  Graphics::Vulkan::Command::Begin(Command, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
  Graphics::Vulkan::Command::CopyBuffer(Command, Staging, Buffer);
  Graphics::Vulkan::Command::End(Command);
}
Light::~Light()
{
  Graphics::Vulkan::Device::WaitForDevice(Core::Engine::GetMainDevice());
  Graphics::Vulkan::CommandPool::Destroy(CommandPool);
  Graphics::Vulkan::Buffer::Destroy(Staging);
  Graphics::Vulkan::Buffer::Destroy(Buffer);
}
void Light::SetStatic(bool isStatic)
{
  this->IsStatic = isStatic;
  if (isStatic)
    this->IsProcessed = false;
}
void Light::SetActive(bool isActive)
{
  this->IsEnabled = isActive;
}
} // namespace Component
} // namespace Tortuga