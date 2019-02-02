#ifndef _SEMAPHORE
#define _SEMAPHORE

#include "../../Core.h"
#include "../../Core.h"
#include "Device.h"

namespace Tortuga
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
}; // namespace Tortuga

#endif