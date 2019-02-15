#ifndef _VULKAN_API_DEVICE
#define _VULKAN_API_DEVICE

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
std::vector<DeviceData> CreateDevices(VulkanData data);
void DestroyDevices(std::vector<DeviceData> data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif