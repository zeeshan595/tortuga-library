#include "Vulkan.hpp"
using namespace Tortuga::Core::Graphics;

Vulkan::Vulkan(std::vector<const char *> extensions)
{
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
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
    }
    if (vkCreateInstance(&instanceCreateInfo, nullptr, &this->_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vulkan instance");
    }

    //Check if all extensions are supported
    {
        uint32_t extensionPropertiesCount = 0;
        std::vector<VkExtensionProperties> extensionProperties;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertiesCount, nullptr);
        extensionProperties.resize(extensionPropertiesCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertiesCount, extensionProperties.data());
        std::cout << "available extensions: " << std::endl;
        for (const auto &extension : extensionProperties)
        {
            std::cout << "\t" << extension.extensionName << std::endl;
        }
    }
}

Vulkan::~Vulkan()
{
    vkDestroyInstance(this->_instance, nullptr);
}

VkInstance Vulkan::GetInstance()
{
    return this->_instance;
}