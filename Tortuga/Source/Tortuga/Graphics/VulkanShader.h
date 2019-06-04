#ifndef _VULKAN_SHADER
#define _VULKAN_SHADER

#include "./VulkanErrorHandler.h"
#include "./VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
  namespace Graphics {
    struct VulkanShader  {
      VkDevice VirtualDevice = VK_NULL_HANDLE;
      VkShaderModule ShaderModule = VK_NULL_HANDLE;
    };

    VulkanShader CreateVulkanShader(VulkanDevice device, std::vector<char> shaderCode);
    void DestroyVulkanShader(VulkanShader data);
  }
}

#endif