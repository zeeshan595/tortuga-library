#include "./Screen.hpp"

#include "../Graphics/DisplaySurface.hpp"

namespace Tortuga
{
namespace Core
{
namespace Screen
{
auto displaySurface = Graphics::DisplaySurface::Create(Core::Engine::GetVulkan(), Core::Engine::GetMainDevice());

void SetWindowTitle(std::string title)
{
  Graphics::DisplaySurface::SetTitle(displaySurface, title);
}
Graphics::Vulkan::Swapchain::Swapchain GetSwapchain()
{
  return displaySurface.Swapchain;
}
void Dispatch()
{
  Graphics::DisplaySurface::Dispatch(displaySurface);
}
bool ShouldClose()
{
  return Graphics::DisplaySurface::ShouldClose(displaySurface);
}
} // namespace Screen
} // namespace Core
} // namespace Tortuga