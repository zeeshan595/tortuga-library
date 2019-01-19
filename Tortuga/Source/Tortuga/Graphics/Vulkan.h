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


    void CheckExtensionSupport();

  public:
    Vulkan(Window* window, const char* applicationName);
    ~Vulkan();
};
}; // namespace Tortuga

#endif