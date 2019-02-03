#ifndef DESCRIPTOR_POOL
#define DESCRIPTOR_POOL

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"

namespace Tortuga
{
class DescriptorPool
{
  private:
    Device *_device;
    VkDescriptorPool _descriptorPool;

  public:
    DescriptorPool(Device *device, uint32_t size);
    ~DescriptorPool();

    VkDescriptorPool GetDescriptorPool() { return _descriptorPool; }
};
}; // namespace Tortuga

#endif