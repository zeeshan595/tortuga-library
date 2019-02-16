#ifndef _VULKAN_API_SHADER
#define _VULKAN_API_SHADER

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
ShaderData CreateShaderFromFile(DeviceData device, std::string filePath, VkShaderStageFlagBits options);
ShaderData CreateShader(DeviceData device, std::vector<char> code, VkShaderStageFlagBits options);
void DestroyShader(ShaderData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif