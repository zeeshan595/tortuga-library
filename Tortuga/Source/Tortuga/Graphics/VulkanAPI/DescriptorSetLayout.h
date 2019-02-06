#ifndef _DESCRIPTOR_SET_LAYOUT
#define _DESCRIPTOR_SET_LAYOUT

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class DescriptorSetLayout
{
private:
  Device *_device;
  VkDescriptorSetLayout _descriptorSetLayout;

public:
  DescriptorSetLayout(Device *device);
  ~DescriptorSetLayout();

  VkDescriptorSetLayout GetDescriptorSetLayout() { return _descriptorSetLayout; }
  uint32_t GetSize() { return 2; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif