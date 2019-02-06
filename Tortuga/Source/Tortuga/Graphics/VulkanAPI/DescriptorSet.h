#ifndef _DESCRIPTOR_SET
#define _DESCRIPTOR_SET

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
  uint32_t _size;
  Device *_device;
  DescriptorPool *_descriptorPool;
  std::vector<VkDescriptorSet> _descriptorSets;

public:
  DescriptorSet(Device *device, DescriptorSetLayout *descriptorSetLayout, DescriptorPool *descriptorPool, uint32_t size);

  std::vector<VkDescriptorSet> GetDescriptorSets() { return _descriptorSets; }
  void UpdateDescriptorSet(std::vector<Buffer *> buffer, VulkanImage *image);
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif