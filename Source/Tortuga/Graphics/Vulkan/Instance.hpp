#ifndef _VULKAN_INSTANCE
#define _VULKAN_INSTANCE

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vector>
#include <shaderc/shaderc.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Instance
{
struct Instance
{
  VkInstance Instance = VK_NULL_HANDLE;
  std::vector<Device::Device> Devices;
  shaderc_compiler_t ShaderCompiler;
  VkDebugUtilsMessengerEXT DebugUtilsMessenger = VK_NULL_HANDLE;
  VkDebugReportCallbackEXT DebugCallbackReport = VK_NULL_HANDLE;
};

Instance Create(bool enableWindowSupport = true);
void Destroy(Instance data);
} // namespace Instance
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif