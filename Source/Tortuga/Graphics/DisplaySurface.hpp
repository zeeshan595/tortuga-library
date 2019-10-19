#ifndef _GRAPHICS_SURFACE
#define _GRAPHICS_SURFACE

#include "./Vulkan/ErrorCheck.hpp"
#include "./Vulkan/Swapchain.hpp"
#include "vulkan/vulkan.h"
#include "../Core/Input.hpp"
#include "./Surface/SurfaceInterface.hpp"
#include "./Surface/WaylandSurface.hpp"

struct Instance;
struct Device;
namespace Tortuga
{
namespace Graphics
{
namespace DisplaySurface
{
struct DisplaySurface
{
  VkInstance Instance;
  Surface::SurfaceInterface *PlatformSurface;
  VkSurfaceKHR Surface;
  Vulkan::Swapchain::Swapchain Swapchain;
};
DisplaySurface Create(Vulkan::Instance::Instance instance, Vulkan::Device::Device device);
void Destroy(DisplaySurface data);
std::vector<const char *> GetVulkanExtensions();
void SetTitle(DisplaySurface data, std::string title);
void Dispatch(DisplaySurface data);
bool ShouldClose(DisplaySurface data);
bool HasPresentSupport(VkPhysicalDevice device, uint32_t familyIndex);
} // namespace DisplaySurface
} // namespace Graphics
} // namespace Tortuga

#endif