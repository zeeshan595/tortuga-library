#ifndef _DESCRIPTOR_LAYOUT
#define _DESCRIPTOR_LAYOUT

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/DescriptorSetLayout.h"

#include "HardwareController.h"

namespace Tortuga
{
namespace Graphics
{
struct DescriptorLayout
{
  HardwareController hardware;
  std::vector<VulkanAPI::DescriptorLayoutData> Layouts;
};
DescriptorLayout CreateDescriptorLayouts(HardwareController hardware);
void DestroyDescriptorLayouts(DescriptorLayout data);
}; // namespace Graphics
}; // namespace Tortuga

#endif