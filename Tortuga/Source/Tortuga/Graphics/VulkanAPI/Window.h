#ifndef _VULKAN_API_WINDOW
#define _VULKAN_API_WINDOW

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
WindowData CreateWindow(std::vector<VulkanAPI::DeviceData> devices, std::string title, uint32_t width, uint32_t height, Uint32 windowFlags);
void DestroyWindow(WindowData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif