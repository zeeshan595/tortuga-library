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

struct WindowData
{
  VulkanAPI::WindowData VulkanWindow;
  std::vector<VulkanAPI::SwapchainData> VulkanSwapchain;
  std::vector<VulkanAPI::DeviceData> VulkanDevicesInUse;
};

WindowData CreateWindow(std::vector<RenderingDevice> devices, std::string title, uint32_t width, uint32_t height, WindowType type);
void DestroyWindow(WindowData data);
}; // namespace Graphics
}; // namespace Tortuga

#endif