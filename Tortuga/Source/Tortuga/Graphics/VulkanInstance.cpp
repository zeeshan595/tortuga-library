#include "VulkanInstance.h"

namespace Tortuga {
namespace Graphics {

VKAPI_ATTR VkBool32 VKAPI_CALL
DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  Console::Info(pCallbackData->pMessage);
  return VK_FALSE;
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

VulkanInstance CreateVulkanInstance() {
  auto data = VulkanInstance();

  // Create Temporary window (need to know what to enable)
  auto tempWindow = CreateWindow("Vulkan Helper", 800, 600);

  std::vector<const char *> extensions = tempWindow.RequiredExtensions;
  std::vector<const char *> validationLayers;

#if DEBUG_MODE
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
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

  //Debugger
#if DEBUG_MODE
  auto debugInfo = VkDebugUtilsMessengerCreateInfoEXT();
  {
    debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugInfo.pfnUserCallback = DebugCallback;
    debugInfo.pUserData = nullptr; // Optional
  }
  ErrorCheck(CreateDebugUtilsMessengerEXT(data.Instance, &debugInfo, nullptr,
                                          &data.Debugger));
#endif

  // Get Physical Devices
  uint32_t devicesCount = 0;
  ErrorCheck(vkEnumeratePhysicalDevices(data.Instance, &devicesCount, nullptr));
  std::vector<VkPhysicalDevice> physicalDevices(devicesCount);
  ErrorCheck(vkEnumeratePhysicalDevices(data.Instance, &devicesCount,
                                        physicalDevices.data()));

  // Init Devices
  CreateSurface(tempWindow, data.Instance);
  for (uint32_t i = 0; i < physicalDevices.size(); i++) {
    auto device = CreateDevice(physicalDevices[i], tempWindow.WindowSurface,
                               data.Instance, validationLayers);

    if (device.IsReady)
      data.Devices.push_back(device);
  }
  // Destroy Window
  DestroyWindow(tempWindow);

  Console::Info("Vulkan is Ready!");
  return data;
}
void DestroyVulkanInstance(VulkanInstance instance) {
  for (uint32_t i = 0; i < instance.Devices.size(); i++) {
    DestroyDevice(instance.Devices[i]);
  }
#if DEBUG_MODE
  DestroyDebugUtilsMessengerEXT(instance.Instance, instance.Debugger, nullptr);
#endif
  vkDestroyInstance(instance.Instance, nullptr);
}
} // namespace Graphics
} // namespace Tortuga