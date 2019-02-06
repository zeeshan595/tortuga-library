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
public:
  enum LayoutType
  {
    Buffer,
    Image
  };

private:
  Device *_device;
  VkDescriptorSetLayout _descriptorSetLayout;
  std::vector<LayoutType> _layouts;

public:
  DescriptorSetLayout(Device *device, std::vector<LayoutType> layouts);
  ~DescriptorSetLayout();

  VkDescriptorSetLayout GetDescriptorSetLayout() { return _descriptorSetLayout; }
  uint32_t GetSize() { return _layouts.size(); }
  std::vector<LayoutType> GetLyouts() { return _layouts; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif