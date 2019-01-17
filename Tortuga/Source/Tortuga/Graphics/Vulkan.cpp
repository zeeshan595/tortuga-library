#include "Vulkan.h"

namespace Tortuga
{
Vulkan::Vulkan(Window *window)
{
    const std::vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    auto extensions = window->GetVulkanInstanceExtensions();
    auto applicationInfo = VkApplicationInfo();
    {
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion = VK_API_VERSION_1_1;
        applicationInfo.pEngineName = "Tortuga";
        applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        applicationInfo.pApplicationName = "";
        applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    }
    auto instanceCreateInfo = VkInstanceCreateInfo();
    {
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
#if DEBUG_MODE
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif
    }
    if (vkCreateInstance(&instanceCreateInfo, nullptr, &this->_instance) != VK_SUCCESS)
    {
        Console::Fatal("Failed to create vulkan instance");
    }
    //auto surface = window->CreateWindowSurface(this->_instance);
}

Vulkan::~Vulkan()
{
    vkDestroyInstance(this->_instance, nullptr);
}
}; // namespace Tortuga