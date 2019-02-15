#ifndef _VULKAN_API
#define _VULKAN_API

#include "../../Core.h"
#include "../../Console.h"

#include "DataStructures.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
VulkanData CreateVulkanInstance();
void DestroyVulkanInstance(VulkanData &data);
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif