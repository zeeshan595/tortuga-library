#ifndef _WAYLAND_DISPLAY
#define _WAYLAND_DISPLAY

#include <iostream>
#include <string.h>
#include <wayland-client.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
struct Display
{
  wl_display *Display = nullptr;
  wl_compositor *Compositor = nullptr;
  wl_shm *Shm = nullptr;
  wl_shell *Shell = nullptr;
  wl_seat *Seat = nullptr;
  wl_pointer *Pointer = nullptr;
  wl_keyboard *Keyboard = nullptr;
  wl_touch *Touch = nullptr;
};
Display CreateWayland();
void DestroyWayland(Display data);
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga

#endif