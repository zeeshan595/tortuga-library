#include "./Instance.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Instance
{
Instance Create(bool enableWindowSupport)
{
  Instance data = {};
  data.ShaderCompiler = shaderc_compiler_initialize();

  if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
  {
    Console::Fatal("Failed to initialize SDL");
  }

  std::vector<const char *> extensions(0);
  SDL_Window *window = nullptr;
  if (enableWindowSupport)
  {
    window = SDL_CreateWindow("Vulkan Helper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 200, SDL_WINDOW_VULKAN | SDL_WINDOW_BORDERLESS);
    if (window == nullptr)
    {
      Console::Fatal("Failed to create SDL Window");
    }
    uint32_t extensionsCount = 0;
    if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionsCount, nullptr))
    {
      Console::Fatal("Failed to get window extensions required for vulkan init");
    }
    extensions.resize(extensionsCount);
    if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionsCount, extensions.data()))
    {
      Console::Fatal("Failed to get window extensions required for vulkan init");
    }
  }

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
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
  }

  ErrorCheck::Callback(vkCreateInstance(&createInfo, nullptr, &data.Instance));

  VkSurfaceKHR surface = VK_NULL_HANDLE;
  if (enableWindowSupport)
  {
    if (!SDL_Vulkan_CreateSurface(window, data.Instance, &surface))
    {
      Console::Fatal("Failed to create vulkan surface for a window");
    }
  }

  uint32_t deviceCount = 0;
  ErrorCheck::Callback(vkEnumeratePhysicalDevices(data.Instance, &deviceCount, nullptr));
  std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
  ErrorCheck::Callback(vkEnumeratePhysicalDevices(data.Instance, &deviceCount, physicalDevices.data()));
  if (deviceCount <= 0)
    Console::Fatal("Failed to locate any graphics device");

  for (uint32_t i = 0; i < deviceCount; i++)
  {
    auto device = Device::Create(physicalDevices[i], surface);
    if (device.IsDeviceCompatible)
      data.Devices.push_back(device);
  }

  if (enableWindowSupport)
  {
    vkDestroySurfaceKHR(data.Instance, surface, nullptr);
    SDL_DestroyWindow(window);
  }

  return data;
}
void Destroy(Instance data)
{
  for (uint32_t i = 0; i < data.Devices.size(); i++)
    Device::Destroy(data.Devices[i]);
  vkDestroyInstance(data.Instance, nullptr);
  shaderc_compiler_release(data.ShaderCompiler);
  SDL_Quit();
}
} // namespace Instance
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga