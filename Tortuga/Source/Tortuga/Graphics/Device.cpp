#include "Device.h"

namespace Tortuga
{
Device::Device(VkPhysicalDevice physicalDevice, std::vector<const char *> validationLayers, VkSurfaceKHR surface)
{
    this->_isReady = false;
    this->_physicalDevice = physicalDevice;
    this->_familyQueues = FindFamilyQueues(physicalDevice, surface);
    this->_swapchainDetails = GetSwapchainSupportDetails(physicalDevice, surface);
    this->_surface = surface;

    //Check to make sure we can use this device for rendering
    if (CheckDeviceSupport(physicalDevice) == false)
    {
        Console::Error("Device does not support geometry shader or basic rendering!");
        return;
    }
    if (CheckSwapchainSupport(this->_swapchainDetails) == false)
    {
        Console::Error("Device does not support swapchains!");
        return;
    }
    if (CheckExtensionSupport(physicalDevice, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}) == false)
    {
        Console::Error("Device does not support required extensions!");
        return;
    }
    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : _familyQueues.GetFamilies())
    {
        auto queueCreateInfo = VkDeviceQueueCreateInfo();
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    auto deviceFeatures = VkPhysicalDeviceFeatures();
    {
        //todo
    }

    auto deviceCreateInfo = VkDeviceCreateInfo();
    {
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.enabledLayerCount = validationLayers.size();
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }
    _device = VK_NULL_HANDLE;
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &_device) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create vulkan device");
    }
    vkGetDeviceQueue(this->_device, _familyQueues.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(this->_device, _familyQueues.presentFamily.value(), 0, &_presentQueue);

    //Setup command pool
    auto poolInfo = VkCommandPoolCreateInfo();
    {
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = _familyQueues.graphicsFamily.value();
        poolInfo.flags = 0; // Optional
    }
    if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
    {
        Console::Error("Failed to create command pool for device!");
        return;
    }
    _isReady = true;
}
Device::~Device()
{
    if (_isReady == false)
        return;

    vkDestroyCommandPool(this->_device, _commandPool, nullptr);
    vkDestroyDevice(this->_device, nullptr);
}

QueueFamilyIndices Device::FindFamilyQueues(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueCount > 0)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamily = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport)
                indices.presentFamily = i;
        }

        if (indices.isComplete())
            break;

        i++;
    }
    return indices;
}

SwapChainSupportDetails Device::GetSwapchainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount > 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount > 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

bool Device::CheckDeviceSupport(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    {
        _deviceName = deviceProperties.deviceName;
        _vendorIdentifier = deviceProperties.vendorID;
    }

    bool isDiscreteGPU = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
    bool geometryCheck = deviceFeatures.geometryShader;
    return isDiscreteGPU && geometryCheck;
}

bool Device::CheckSwapchainSupport(SwapChainSupportDetails details)
{
    return !details.formats.empty() && !details.presentModes.empty();
}

bool Device::CheckExtensionSupport(VkPhysicalDevice device, std::vector<const char *> extensions)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());

    for (const auto &extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

}; // namespace Tortuga