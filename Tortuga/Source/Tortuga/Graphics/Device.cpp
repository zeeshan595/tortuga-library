#include "Device.h"

namespace Tortuga
{
Device::Device(VkPhysicalDevice physicalDevice, std::vector<const char *> validationLayers, std::vector<const char *> extensions)
{
    this->_physicalDevice = physicalDevice;

    auto deviceCreateInfo = VkDeviceCreateInfo();
    {
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.enabledLayerCount = validationLayers.size();
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        deviceCreateInfo.enabledExtensionCount = extensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
    }
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &this->_device) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create vulkan device");
    }
}
Device::~Device()
{
    vkDestroyDevice(this->_device, nullptr);
}
}; // namespace Tortuga