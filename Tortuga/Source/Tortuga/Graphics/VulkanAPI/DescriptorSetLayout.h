#ifndef _DESCRIPTOR_SET_LAYOUT
#define _DESCRIPTOR_SET_LAYOUT

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"

namespace Tortuga
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
};
}; // namespace Tortuga

#endif