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
Instance Create(bool enableWindowSupport)
{
  Instance data = {};
  data.ShaderCompiler = shaderc_compiler_initialize();

  std::vector<const char *> extensions;
  if (enableWindowSupport)
  {
    const auto windowExtensions = DisplaySurface::GetVulkanExtensions();
    for (auto e : windowExtensions)
      extensions.push_back(e);
  }
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

  DisplaySurface::DisplaySurface queryDisplaySurface;
  if (enableWindowSupport)
    queryDisplaySurface = DisplaySurface::Create(data);

  uint32_t deviceCount = 0;
  ErrorCheck::Callback(vkEnumeratePhysicalDevices(data.Instance, &deviceCount, nullptr));
  std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
  ErrorCheck::Callback(vkEnumeratePhysicalDevices(data.Instance, &deviceCount, physicalDevices.data()));
  if (deviceCount <= 0)
    Console::Fatal("Failed to locate any graphics device");

  for (uint32_t i = 0; i < deviceCount; i++)
  {
    auto device = Device::Create(physicalDevices[i], queryDisplaySurface.Surface);
    if (device.IsDeviceCompatible)
      data.Devices.push_back(device);
  }

  if (enableWindowSupport)
    Destroy(queryDisplaySurface);

  return data;
}
void Destroy(Instance data)
{
  if (data.Instance == VK_NULL_HANDLE)
    return;

  for (uint32_t i = 0; i < data.Devices.size(); i++)
    Device::Destroy(data.Devices[i]);

  vkDestroyInstance(data.Instance, nullptr);
  shaderc_compiler_release(data.ShaderCompiler);
}
} // namespace Instance
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga