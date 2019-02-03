#ifndef _SEMAPHORE
#define _SEMAPHORE

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class Semaphore
{
private:
  Device *_device;
  VkSemaphore _semaphore;

public:
  Semaphore(Device *device);
  ~Semaphore();

  VkSemaphore GetSemaphore() { return _semaphore; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif