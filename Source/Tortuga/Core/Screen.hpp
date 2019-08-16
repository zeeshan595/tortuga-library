#ifndef _SCREEN
#define _SCREEN

#include "./Engine.hpp"
#include "../Graphics/Vulkan/Window.hpp"
#include "../Graphics/Vulkan/Swapchain.hpp"

namespace Tortuga
{
namespace Core
{
namespace Screen
{
SDL_Event PollEvents();
void SetWindowTitle(std::string title);
void ResizeWindow(uint32_t width, uint32_t height);
std::vector<int32_t> GetWindowSize();
Graphics::Vulkan::Window::Window GetWindow();
Graphics::Vulkan::Swapchain::Swapchain GetSwapchain();
} // namespace Screen
} // namespace Core
} // namespace Tortuga

#endif