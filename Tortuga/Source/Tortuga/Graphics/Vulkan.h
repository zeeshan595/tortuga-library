#ifndef _VULKAN
#define _VULKAN

#include <vector>
#include <vulkan.h>

#include "../Core.h"
#include "Window.h"
#include "Device.h"

namespace Tortuga
{
class Vulkan
{
  private:
    VkInstance _instance;
    //std::vector<Device> _devices;

  public:
    Vulkan(Window* window);
    ~Vulkan();
};
}; // namespace Tortuga

#endif