#include "./Surface.hpp"

void shell_surface_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial)
{
  wl_shell_surface_pong(shell_surface, serial);
}

void shell_surface_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height)
{
}

const struct wl_shell_surface_listener shell_surface_listener = {
    .ping = shell_surface_ping,
    .configure = shell_surface_configure,
};

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
Surface CreateSurface(Display wayland)
{
  auto data = Surface();
  data.Surface = wl_compositor_create_surface(wayland.Compositor);
  if (data.Surface == nullptr)
  {
    perror("failed to create wayland surface");
    return data;
  }
  data.ShellSurface = wl_shell_get_shell_surface(wayland.Shell, data.Surface);
  if (data.ShellSurface == nullptr)
  {
    perror("failed to create wayland shell surface");
    return data;
  }

  wl_shell_surface_add_listener(data.ShellSurface, &shell_surface_listener, 0);
  wl_shell_surface_set_toplevel(data.ShellSurface);

  return data;
}
void DestroySurface(Surface data)
{
  wl_shell_surface_destroy(data.ShellSurface);
  wl_surface_destroy(data.Surface);
}
void BindSurfaceWithBuffer(Surface data, Buffer buffer)
{
  wl_surface_attach(data.Surface, buffer.Buffer, 0, 0);
  wl_surface_commit(data.Surface);
}
void SetTitle(Surface data, const char *title)
{
  wl_shell_surface_set_title(data.ShellSurface, title);
}
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga