#include "DescriptorSet.h"

namespace Tortuga
{
namespace Graphics
{
void ConfigureDescriptorSet(DescriptorSet data, Buffer buffer, uint32_t binding)
{
  for (uint32_t i = 0; i < data.VulkanDescriptorSets.size(); i++)
  {
    VulkanAPI::ConfigureDescriptorSetBuffer(data.VulkanDescriptorSets[i], buffer.VulkanBuffer[i], binding);
  }
}
DescriptorSet CreateDescriptorSet(HardwareController hardware, DescriptorLayout layout, DescriptorType type, DescriptorPool pool)
{
  auto data = DescriptorSet();
  data.Hardware = hardware;
  data.Type = type;

  data.VulkanDescriptorSets.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    switch (type)
    {
    default:
    case DESCRIPTOR_TYPE_UNIFORM:
      data.VulkanDescriptorSets[i] = VulkanAPI::CreateDescriptorSet(
          hardware.Devices[i].VulkanDevice,
          layout.UniformLayouts[i],
          pool.VulkanDescriptorPools[i]);
      break;
    case DESCRIPTOR_TYPE_IMAGE:
      data.VulkanDescriptorSets[i] = VulkanAPI::CreateDescriptorSet(
          hardware.Devices[i].VulkanDevice,
          layout.ImageLayouts[i],
          pool.VulkanDescriptorPools[i]);
      break;
    }
  }

  return data;
}
DescriptorPool CreateDescriptorPool(HardwareController hardware, DescriptorType type, uint32_t descriptorsAmount)
{
  auto data = DescriptorPool();
  data.Hardware = hardware;
  data.Type = type;

  VkDescriptorType vulkanDescriptorType;
  switch (type)
  {
  default:
  case DESCRIPTOR_TYPE_UNIFORM:
    vulkanDescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    break;
  case DESCRIPTOR_TYPE_IMAGE:
    vulkanDescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    break;
  }

  data.VulkanDescriptorPools.resize(hardware.Devices.size());
  for (uint32_t i = 0; i < hardware.Devices.size(); i++)
  {
    data.VulkanDescriptorPools[i] = VulkanAPI::CreateDescriptorPool(
        hardware.Devices[i].VulkanDevice,
        vulkanDescriptorType,
        descriptorsAmount);
  }
  return data;
}
void DestroyDescriptorPool(DescriptorPool data)
{
  for (uint32_t i = 0; i < data.Hardware.Devices.size(); i++)
  {
    VulkanAPI::DestroyDescriptorPool(data.VulkanDescriptorPools[i]);
  }
}
}; // namespace Graphics
}; // namespace Tortuga
