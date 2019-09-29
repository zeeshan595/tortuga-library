#ifndef _VULKAN_ERROR_CHECK
#define _VULKAN_ERROR_CHECK

#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan.h>

#include "../../Core/Console.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace ErrorCheck
{
void Callback(VkResult result);


VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage, void *pUserData);
VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
VkResult CreateDebugReportCallback(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pDebugMessenger);
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT debugMessenger, const VkAllocationCallbacks *pAllocator);
} // namespace ErrorCheck
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif