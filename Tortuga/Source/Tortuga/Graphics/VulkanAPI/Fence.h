#ifndef _FENCE
#define _FENCE

#include "../../Core.h"
#include "../../Core.h"
#include "Device.h"

namespace Tortuga
{
class Fence
{
private:
  Device *_device;
  VkFence _fence;

public:
  Fence(Device *device);
  ~Fence();

  VkFence &GetFence() { return _fence; }
};
}; // namespace Tortuga

#endif