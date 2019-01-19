#include "Vulkan.h"

namespace Tortuga
{
Vulkan::Vulkan(Window *window, const char* applicationName)
{
    std::vector<const char *> validationLayers;
#if DEBUG_MODE
    validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif

    auto extensions = window->GetVulkanInstanceExtensions();

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
    if (vkCreateInstance(&instanceCreateInfo, nullptr, &this->_instance) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create vulkan instance");
    }
    CheckExtensionSupport();

    auto surface = window->CreateWindowSurface(this->_instance);
}

Vulkan::~Vulkan()
{
    vkDestroyInstance(this->_instance, nullptr);
}

void Vulkan::CheckExtensionSupport()
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

}; // namespace Tortuga