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
void ConfigureDescriptorSet(DescriptorSet data, Buffer buffer, uint32_t binding);
DescriptorSet CreateDescriptorSet(HardwareController hardware, DescriptorLayout layout, DescriptorType type, DescriptorPool pool);
DescriptorPool CreateDescriptorPool(HardwareController hardware, DescriptorType type, uint32_t descriptorsAmount);
void DestroyDescriptorPool(DescriptorPool data);
}; // namespace Graphics
}; // namespace Tortuga

#endif