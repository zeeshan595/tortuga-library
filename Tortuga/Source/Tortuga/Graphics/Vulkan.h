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
  VkDebugUtilsMessengerEXT _debugReport;
  VkInstance _instance;
  //std::vector<Device> _devices;
  void GetEnabledExtensions();
  void CheckValidationSupport(std::vector<const char *> validationLayers);
  void CreateDebugger();
  void DestroyDebugger();

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT *data,
      void *userData);

public:
  Vulkan(Window *window, const char *applicationName);
  ~Vulkan();
};
}; // namespace Tortuga

#endif