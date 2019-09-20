#ifndef _WAYLAND_SURFACE
#define _WAYLAND_SURFACE

#include "./Display.hpp"
#include "./Buffer.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
struct Surface
{
  wl_surface *Surface;
  wl_shell_surface *ShellSurface;
};
Surface CreateSurface(Display wayland);
void DestroySurface(Surface data);
void BindSurfaceWithBuffer(Surface data, Buffer buffer);
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga

#endif