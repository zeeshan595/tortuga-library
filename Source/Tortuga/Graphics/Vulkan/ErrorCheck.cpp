#include "./ErrorCheck.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace ErrorCheck
{
void Callback(VkResult result)
{
  if (result != VK_SUCCESS)
  {
    Console::Fatal("invalid response from vulkan function: {0}", result);
  }
}
VkBool32 DebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData)
{
  Console::Info(pMessage);
  return VK_TRUE;
};

VkBool32 DebugUtilCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
  Console::Info(pCallbackData->pMessage);
  return VK_TRUE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func == nullptr)
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  
  return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func == nullptr)
    return;

  func(instance, debugMessenger, pAllocator);
}

VkResult CreateDebugReportCallback(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pDebugMessenger)
{
  auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
  if (func == nullptr)
    return VK_ERROR_EXTENSION_NOT_PRESENT;

  return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
  auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
  if (func == nullptr)
    return;
    
  func(instance, debugMessenger, pAllocator);
}
} // namespace ErrorCheck
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga