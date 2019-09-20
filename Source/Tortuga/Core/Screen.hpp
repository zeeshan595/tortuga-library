#ifndef _SCREEN
#define _SCREEN

#include "./Engine.hpp"
#include "../Graphics/DisplaySurface.hpp"
#include "../Graphics/Vulkan/Swapchain.hpp"

namespace Tortuga
{
namespace Core
{
namespace Screen
{
void SetWindowTitle(std::string title);
Graphics::Vulkan::Swapchain::Swapchain GetSwapchain();
} // namespace Screen
} // namespace Core
} // namespace Tortuga

#endif