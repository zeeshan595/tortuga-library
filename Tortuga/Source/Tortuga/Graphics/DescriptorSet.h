#ifndef _DESCRIPTOR_SET
#define _DESCRIPTOR_SET

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/DescriptorPool.h"
#include "VulkanAPI/DescriptorSet.h"

#include "HardwareController.h"
#include "DescriptorLayout.h"
#include "Buffer.h"

namespace Tortuga
{
namespace Graphics
{
enum DescriptorType
{
  DESCRIPTOR_TYPE_UNIFORM,
  DESCRIPTOR_TYPE_IMAGE
};
struct DescriptorSet
{
  HardwareController Hardware;
  DescriptorType Type;
  std::vector<VulkanAPI::DescriptorSetData> VulkanDescriptorSets;
};
struct DescriptorPool
{
  HardwareController Hardware;
  DescriptorType Type;
  std::vector<VulkanAPI::DescriptorPoolData> VulkanDescriptorPools;
};
DescriptorSet CreateDescriptorSet(HardwareController hardware, DescriptorLayout layout, DescriptorType type, DescriptorPool pool, Buffer buffer);
DescriptorPool CreateDescriptorPool(HardwareController hardware, DescriptorType type, uint32_t descriptorsAmount);
void DestroyDescriptorPool(DescriptorPool data);
}; // namespace Graphics
}; // namespace Tortuga

#endif