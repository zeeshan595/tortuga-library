#include "VulkanInstance.h"

namespace Tortuga {
namespace Graphics {
VulkanInstance CreateVulkanInstance() {
  auto data = VulkanInstance();

  // Setup Vulkan Instance
  auto tempWindow = Window();
  auto applicationInfo = VkApplicationInfo();
  {
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pEngineName = "Tortuga";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pApplicationName = "Tortuga Application";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_VERSION_1_1;
  }

  auto createInfo = VkInstanceCreateInfo();
  {
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = tempWindow.RequiredExtensions.size();
    createInfo.ppEnabledExtensionNames = tempWindow.RequiredExtensions.data();
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }
  ErrorCheck(vkCreateInstance(&createInfo, nullptr, &data.Instance));

  // Get Physical Devices
  uint32_t devicesCount = 0;
  ErrorCheck(vkEnumeratePhysicalDevices(data.Instance, &devicesCount, nullptr));
  data.PhysicalDevices.resize(devicesCount);
  ErrorCheck(vkEnumeratePhysicalDevices(data.Instance, &devicesCount,
                                        data.PhysicalDevices.data()));

  return data;
}
void DestroyVulkanInstance(VulkanInstance instance) {
  vkDestroyInstance(instance.Instance, nullptr);
}
} // namespace Graphics
} // namespace Tortuga