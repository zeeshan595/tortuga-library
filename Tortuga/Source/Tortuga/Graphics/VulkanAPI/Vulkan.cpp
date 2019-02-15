#include "Vulkan.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
  Console::Info(pCallbackData->pMessage);
  return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    func(instance, debugMessenger, pAllocator);
  }
}

VulkanData CreateVulkanInstance()
{
  auto data = VulkanData();

  std::vector<const char *> extensions = {};
  std::vector<const char *> layers = {};

  //Setup exteions
  {
    auto window = SDL_CreateWindow(
        "VK Instance Helper",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_VULKAN);
    uint32_t extensionsCount = 0;
    if (SDL_Vulkan_GetInstanceExtensions(window, &extensionsCount, nullptr) == false)
    {
      Console::Fatal("Failed to get window extensions size!");
    }
    extensions.resize(extensionsCount);
    if (SDL_Vulkan_GetInstanceExtensions(window, &extensionsCount, extensions.data()) == false)
    {
      Console::Fatal("Failed to get window extensions!");
    }
    SDL_DestroyWindow(window);
  }

#if DEBUG_MODE
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  layers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif

  auto applicationInfo = VkApplicationInfo();
  {
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion = VK_API_VERSION_1_1;
    applicationInfo.pEngineName = "Tortuga Rendering Engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pApplicationName = "Tortuga Rendering Application";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  }

  auto instanceInfo = VkInstanceCreateInfo();
  {
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &applicationInfo;
    instanceInfo.enabledLayerCount = layers.size();
    instanceInfo.ppEnabledLayerNames = layers.data();
    instanceInfo.enabledExtensionCount = extensions.size();
    instanceInfo.ppEnabledExtensionNames = extensions.data();
  }
  if (vkCreateInstance(&instanceInfo, nullptr, &data.Instance) != VK_SUCCESS)
  {
    Console::Fatal("Failed to create vulkan instance!");
  }

  data.InstanceExtensions = extensions;
  data.ValidationLayers = layers;

#if DEBUG_MODE
  auto debugInfo = VkDebugUtilsMessengerCreateInfoEXT();
  {
    debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugInfo.pfnUserCallback = DebugCallback;
    debugInfo.pUserData = nullptr; // Optional
  }
  CreateDebugUtilsMessengerEXT(data.Instance, &debugInfo, nullptr, &data.Debugger);
#endif

  return data;
}
void DestroyVulkanInstance(VulkanData &data)
{
#if DEBUG_MODE
  DestroyDebugUtilsMessengerEXT(data.Instance, data.Debugger, nullptr);
#endif
  vkDestroyInstance(data.Instance, nullptr);
}
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga