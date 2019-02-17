#ifndef _WINDOW
#define _WINDOW

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/Window.h"
#include "VulkanAPI/Swapchain.h"

#include "RenderingEngine.h"

namespace Tortuga
{
namespace Graphics
{
enum WindowType
{
  FullScreen,
  Windowed,
  ResizeableWindowed,
  BorderlessWindowed
};

struct Window
{
  uint32_t Width;
  uint32_t Height;
  std::vector<uint32_t> devicesViewportSize;
  VulkanAPI::WindowData VulkanWindow;
  std::vector<VulkanAPI::SwapchainData> VulkanSwapchain;
  std::vector<VulkanAPI::DeviceData> VulkanDevicesInUse;
};

Window CreateWindow(std::vector<RenderingDevice> devices, std::string title, uint32_t width, uint32_t height, WindowType type);
void DestroyWindow(Window data);
}; // namespace Graphics
}; // namespace Tortuga

#endif