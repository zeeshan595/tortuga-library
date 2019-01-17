#ifndef _DEVICE
#define _DEVICE

#include <vector>
#include <vulkan.h>

#include "../Console.h"

namespace Tortuga
{
class Device
{
  private:
    VkPhysicalDevice _physicalDevice;
    VkDevice _device;

  public:
    Device(VkPhysicalDevice physicalDevice, std::vector<const char *> validationLayers, std::vector<const char *> extensions);
    ~Device();
};
}; // namespace Tortuga

#endif