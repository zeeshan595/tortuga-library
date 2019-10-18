#ifndef _VULKAN_INSTANCE
#define _VULKAN_INSTANCE

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Instance
{
struct Instance
{
  VkInstance Instance = VK_NULL_HANDLE;
  std::vector<Device::Device> Devices;
  VkDebugUtilsMessengerEXT DebugUtilsMessenger = VK_NULL_HANDLE;
  VkDebugReportCallbackEXT DebugCallbackReport = VK_NULL_HANDLE;
};

Instance Create();
void Destroy(Instance data);
} // namespace Instance
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif