#ifndef _GRAPHICS_SURFACE
#define _GRAPHICS_SURFACE

#include "./SDL2/SDL.h"
#include "./SDL2/SDL_vulkan.h"

#include "./Vulkan/ErrorCheck.hpp"
#include "./Vulkan/Swapchain.hpp"
#include "vulkan/vulkan.h"
#include "../Core/Input.hpp"

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
  SDL_Window *Window;
  VkSurfaceKHR Surface;
  Vulkan::Swapchain::Swapchain Swapchain;
};
DisplaySurface Create(Vulkan::Instance::Instance instance, Vulkan::Device::Device device);
void Destroy(DisplaySurface data);
void SetTitle(DisplaySurface data, std::string title);
void Dispatch(DisplaySurface data);
} // namespace DisplaySurface
} // namespace Graphics
} // namespace Tortuga

#endif