#include "Vulkan.h"

namespace Tortuga
{
Vulkan::Vulkan(Window *window, const char *applicationName)
{
    this->_window = window;

    std::vector<const char *> validationLayers;
    auto extensions = window->GetVulkanInstanceExtensions();
#if DEBUG_MODE
    validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    CheckValidationSupport(validationLayers);

    auto applicationInfo = VkApplicationInfo();
    {
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion = VK_API_VERSION_1_1;
        applicationInfo.pEngineName = "Tortuga";
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pApplicationName = applicationName;
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    }
    auto instanceCreateInfo = VkInstanceCreateInfo();
    {
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    }
    if (vkCreateInstance(&instanceCreateInfo, nullptr, &_instance) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create vulkan instance");
    }
    GetEnabledExtensions();

#if DEBUG_MODE
    CreateDebugger();
#endif

    //Find supported GPUs
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
    if (deviceCount == 0)
        Console::Fatal("Failed to find GPUs with vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

    //Get surface
    _surface = window->CreateWindowSurface(this->_instance);

    //Setup Devices
    auto supportedDevices = std::vector<VkPhysicalDevice>();
    for (const auto &device : devices)
    {
        //setup new device and make sure it is in ready state
        auto newDevice = new Device(device, validationLayers, _surface);
        if (newDevice->IsReady())
            _devices.push_back(newDevice);
    }
    if (_devices.size() == 0)
        Console::Fatal("Failed to find compatible GPUs");
}

Vulkan::~Vulkan()
{
    for (uint32_t i = 0; i < this->_devices.size(); i++)
        delete this->_devices[i];

#if DEBUG_MODE
    DestroyDebugger();
#endif
    vkDestroyInstance(_instance, nullptr);
}

void Vulkan::GetEnabledExtensions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    Console::Info("Supported extensions:");
    for (const auto &extension : extensions)
    {
        Console::Info("\t{0}", extension.extensionName);
    }
}

void Vulkan::CheckValidationSupport(std::vector<const char *> validationLayers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
            Console::Fatal("Failed to check validation support: {0}", layerName);
    }
}

void Vulkan::CreateDebugger()
{
    auto debugFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
    if (debugFunc == nullptr)
    {
        Console::Fatal("Failed to find 'create vulkan debug utils'");
    }
    auto debugCreateInfo = VkDebugUtilsMessengerCreateInfoEXT();
    {
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = DebugCallback;
        debugCreateInfo.pUserData = nullptr; // Optional
    }
    if (debugFunc(_instance, &debugCreateInfo, nullptr, &_debugReport) != VK_SUCCESS)
    {
        Console::Fatal("Failed to setup vulkan debug utils");
    }
}

void Vulkan::DestroyDebugger()
{
    auto debugFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");
    if (debugFunc == nullptr)
    {
        Console::Fatal("Failed to find 'destroy vulkan debug utils'");
    }
    debugFunc(_instance, _debugReport, nullptr);
}

VkBool32 Vulkan::DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *data,
    void *userData)
{
    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        Console::Fatal(data->pMessage);
        return VK_FALSE;
    }
    else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        Console::Warning(data->pMessage);
    }

    Console::Info(data->pMessage);
    return VK_TRUE;
}

}; // namespace Tortuga