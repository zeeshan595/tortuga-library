#ifndef _VULKAN_API_DESCRIPTOR_SET
#define _VULKAN_API_DESCRIPTOR_SET

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "DescriptorSetLayout.h"
#include "DescriptorPool.h"
#include "Buffer.h"
#include "VulkanImage.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class DescriptorSet
{
private:
  Device *_device;
  DescriptorPool *_descriptorPool;
  VkDescriptorSet _descriptorSet;

public:
  DescriptorSet(Device *device, DescriptorSetLayout *descriptorSetLayout, DescriptorPool *descriptorPool);

  VkDescriptorSet GetDescriptorSet() { return _descriptorSet; }
  void UpdateDescriptorSet(Buffer *buffer, VulkanImage *image);
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif