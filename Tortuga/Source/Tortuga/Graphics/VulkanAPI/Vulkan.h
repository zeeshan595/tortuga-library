#ifndef _VULKAN
#define _VULKAN

#include "../Core.h"
#include "QueueFamilyIndices.h"
#include "Window.h"
#include "Device.h"

namespace Tortuga
{
class Vulkan
{
private:
  VkDebugUtilsMessengerEXT _debugReport;
  VkInstance _instance;
  Window *_window;
  VkSurfaceKHR _surface;
  std::vector<Device *> _devices;

  void GetEnabledExtensions();
  void CheckValidationSupport(std::vector<const char *> validationLayers);

  void CreateDebugger();
  void DestroyDebugger();

  //Vulkan Debug API
  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT *data,
      void *userData);

public:
  Vulkan(Window *window, const char *applicationName);
  ~Vulkan();

  VkInstance GetVulkanInstance() { return _instance; }
  Window *GetWindow() { return _window; }
  VkSurfaceKHR GetSurface() { return _surface; }
  std::vector<Device *> GetDevices() { return _devices; }
};
}; // namespace Tortuga

#endif