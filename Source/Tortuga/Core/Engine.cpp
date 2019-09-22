#include "./Engine.hpp"

namespace Tortuga
{
namespace Core
{
namespace Engine
{
const auto VulkanInstance = Graphics::Vulkan::Instance::Create();
const auto VertexUniformBufferLayout = Graphics::Vulkan::DescriptorLayout::Create(GetMainDevice(), 1, VK_SHADER_STAGE_VERTEX_BIT);
const auto VertexAndFragmentUniformBufferLayout = Graphics::Vulkan::DescriptorLayout::Create(GetMainDevice(), 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT);
const auto FragmentCombinedImageSampleLayout = Graphics::Vulkan::DescriptorLayout::Create(GetMainDevice(), 1, VK_SHADER_STAGE_FRAGMENT_BIT, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

int32_t BestDevice = -1;
void UpdateBestDevice()
{
  if (BestDevice != -1)
    return;

  BestDevice = 0;
  for (uint32_t i = 1; i < VulkanInstance.Devices.size(); i++)
  {
    if (VulkanInstance.Devices[i].Score > VulkanInstance.Devices[BestDevice].Score)
      BestDevice = i;
  }
}

Graphics::Vulkan::Instance::Instance GetVulkan()
{
  return VulkanInstance;
}
std::vector<Graphics::Vulkan::Device::Device> GetAllDevices()
{
  return VulkanInstance.Devices;
}
Graphics::Vulkan::Device::Device GetMainDevice()
{
  UpdateBestDevice();
  return VulkanInstance.Devices[BestDevice];
}
uint32_t GetMainDeviceIndex()
{
  UpdateBestDevice();
  return BestDevice;
}
Graphics::Vulkan::DescriptorLayout::DescriptorLayout GetVertexUniformBufferLayout()
{
  return VertexUniformBufferLayout;
}
Graphics::Vulkan::DescriptorLayout::DescriptorLayout GetVertexAndFragmentUniformBufferLayout()
{
  return VertexAndFragmentUniformBufferLayout;
}
Graphics::Vulkan::DescriptorLayout::DescriptorLayout GetFragmentCombinedImageSampleLayout()
{
  return FragmentCombinedImageSampleLayout;
}
} // namespace Engine
} // namespace Core
} // namespace Tortuga