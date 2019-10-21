#include "./Instance.hpp"

#include "../DisplaySurface.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Instance
{
Instance Create()
{
  Instance data = {};
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    Console::Fatal("SDL: failed to init {0}", SDL_GetError());
  }

  //create helper window
  const auto helperWindow = SDL_CreateWindow("tortuga helper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN);
  uint32_t windowExtensionsCount = 0;
  if (!SDL_Vulkan_GetInstanceExtensions(helperWindow, &windowExtensionsCount, nullptr))
    Console::Fatal("SDL: failed to get window extensions");
  std::vector<const char *> windowExtensions(windowExtensionsCount);
  if (!SDL_Vulkan_GetInstanceExtensions(helperWindow, &windowExtensionsCount, windowExtensions.data()))
    Console::Fatal("SDL: failed to get window extensions");

  std::vector<const char *> extensions = windowExtensions;
  std::vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};
  extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  VkApplicationInfo appInfo = {};
  {
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr, appInfo.pEngineName = "Tortuga";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pApplicationName = "Tortuga Application";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_1;
  }

  VkInstanceCreateInfo createInfo = {};
  {
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
  }
  ErrorCheck::Callback(vkCreateInstance(&createInfo, nullptr, &data.Instance));

  uint32_t deviceCount = 0;
  ErrorCheck::Callback(vkEnumeratePhysicalDevices(data.Instance, &deviceCount, nullptr));
  std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
  ErrorCheck::Callback(vkEnumeratePhysicalDevices(data.Instance, &deviceCount, physicalDevices.data()));
  if (deviceCount <= 0)
    Console::Fatal("Failed to locate any graphics device");

  //create helper window surface
  VkSurfaceKHR helperSurface = VK_NULL_HANDLE;
  if (!SDL_Vulkan_CreateSurface(helperWindow, data.Instance, &helperSurface))
    Console::Fatal("SDL: failed to setup surface, it is required to initialize devices");

  for (uint32_t i = 0; i < deviceCount; i++)
  {
    auto device = Device::Create(helperSurface, physicalDevices[i]);
    if (device.IsDeviceCompatible)
      data.Devices.push_back(device);
  }

  vkDestroySurfaceKHR(data.Instance, helperSurface, nullptr);
  SDL_DestroyWindow(helperWindow);

  return data;
}
void Destroy(Instance data)
{
  if (data.Instance == VK_NULL_HANDLE)
    return;

  for (uint32_t i = 0; i < data.Devices.size(); i++)
    Device::Destroy(data.Devices[i]);

  vkDestroyInstance(data.Instance, nullptr);
  SDL_Quit();
}
} // namespace Instance
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga