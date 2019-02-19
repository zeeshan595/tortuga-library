#ifndef _VULKAN_API_WINDOW
#define _VULKAN_API_WINDOW

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
WindowData CreateWindow(VulkanAPI::DeviceData device, std::string title, uint32_t width, uint32_t height, Uint32 windowFlags);
void DestroyWindow(WindowData data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif