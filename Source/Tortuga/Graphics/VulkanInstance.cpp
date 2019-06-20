#include "VulkanInstance.h"

namespace Tortuga {
namespace Graphics {

VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
    VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
    uint64_t object, size_t location, int32_t messageCode,
    const char *pLayerPrefix, const char *pMessage, void *pUserData) {
  Console::Info(pMessage);
  return VK_TRUE;
};

VKAPI_ATTR VkBool32 VKAPI_CALL
DebugUtilCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                  void *pUserData) {
  Console::Info(pCallbackData->pMessage);
  return VK_TRUE;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

VkResult
CreateDebugReportCallback(VkInstance instance,
                          const VkDebugReportCallbackCreateInfoEXT *pCreateInfo,
                          const VkAllocationCallbacks *pAllocator,
                          VkDebugReportCallbackEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugReportCallbackEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugReportCallbackEXT(VkInstance instance,
                                   VkDebugReportCallbackEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugReportCallbackEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

VulkanInstance CreateVulkanInstance() {
  auto data = VulkanInstance();

  // Create Temporary window (need to know what to enable)
  auto tempWindow = CreateVulkanWindow("Vulkan Helper", 800, 600);

  std::vector<const char *> extensions = tempWindow.RequiredExtensions;
  std::vector<const char *> validationLayers;

#if DEBUG_MODE
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif

  // Setup Vulkan Instance
  auto applicationInfo = VkApplicationInfo();
  {
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pEngineName = "Tortuga";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pApplicationName = "Tortuga Application";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_1;
  }

  auto createInfo = VkInstanceCreateInfo();
  {
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
  }
  ErrorCheck(vkCreateInstance(&createInfo, nullptr, &data.Instance));

  // Debugger
#if DEBUG_MODE
  auto utilInfo = VkDebugUtilsMessengerCreateInfoEXT();
  {
    utilInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    utilInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    utilInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    utilInfo.pfnUserCallback = DebugUtilCallback;
    utilInfo.pUserData = nullptr; // Optional
  }
  ErrorCheck(CreateDebugUtilsMessengerEXT(data.Instance, &utilInfo, nullptr,
                                          &data.DebugUtil));

  auto reportInfo = VkDebugReportCallbackCreateInfoEXT();
  {
    reportInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    reportInfo.pNext = 0;
#if INTENSE_DEBUG_MODE
    reportInfo.flags =
        VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
#else
    reportInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT |
                       VK_DEBUG_REPORT_ERROR_BIT_EXT |
                       VK_DEBUG_REPORT_DEBUG_BIT_EXT;
#endif
    reportInfo.pfnCallback = DebugReportCallback;
    reportInfo.pUserData = 0;
  }
  ErrorCheck(CreateDebugReportCallback(data.Instance, &reportInfo, nullptr,
                                       &data.DebugReport));
#endif

  // Get Physical Devices
  uint32_t devicesCount = 0;
  ErrorCheck(vkEnumeratePhysicalDevices(data.Instance, &devicesCount, nullptr));
  std::vector<VkPhysicalDevice> physicalDevices(devicesCount);
  ErrorCheck(vkEnumeratePhysicalDevices(data.Instance, &devicesCount,
                                        physicalDevices.data()));

  // Init Devices
  CreateVulkanSurface(tempWindow, data.Instance);
  for (uint32_t i = 0; i < physicalDevices.size(); i++) {
    auto device =
        CreateVulkanDevice(physicalDevices[i], tempWindow.WindowSurface,
                           data.Instance, validationLayers);

    if (device.IsReady)
      data.Devices.push_back(device);
  }
  // Destroy Window
  DestroyVulkanWindow(tempWindow);

  Console::Info("Vulkan is Ready!");
  return data;
}
void DestroyVulkanInstance(VulkanInstance instance) {
  for (uint32_t i = 0; i < instance.Devices.size(); i++) {
    DestroyVulkanDevice(instance.Devices[i]);
  }
#if DEBUG_MODE
  DestroyDebugUtilsMessengerEXT(instance.Instance, instance.DebugUtil, nullptr);
  DestroyDebugReportCallbackEXT(instance.Instance, instance.DebugReport,
                                nullptr);
#endif
  vkDestroyInstance(instance.Instance, nullptr);
}
} // namespace Graphics
} // namespace Tortuga