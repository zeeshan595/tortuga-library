#include "./Display.hpp"

wl_compositor *compositor = nullptr;
wl_shm *shm = nullptr;
wl_shell *shell = nullptr;
wl_seat *seat = nullptr;
wl_pointer *pointer = nullptr;
wl_keyboard *keyboard = nullptr;

void RegistryGlobal(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
  if (strcmp(interface, wl_compositor_interface.name) == 0)
    compositor = (wl_compositor *)wl_registry_bind(registry, name, &wl_compositor_interface, version);
  else if (strcmp(interface, wl_shm_interface.name) == 0)
    shm = (wl_shm *)wl_registry_bind(registry, name, &wl_shm_interface, version);
  else if (strcmp(interface, wl_shell_interface.name) == 0)
    shell = (wl_shell *)wl_registry_bind(registry, name, &wl_shell_interface, version);
  else if (strcmp(interface, wl_seat_interface.name))
    seat = (wl_seat *)wl_registry_bind(registry, name, &wl_seat_interface, version);
  else if (strcmp(interface, wl_pointer_interface.name))
    pointer = (wl_pointer *)wl_registry_bind(registry, name, &wl_pointer_interface, version);
  else if (strcmp(interface, wl_keyboard_interface.name))
    keyboard = (wl_keyboard *)wl_registry_bind(registry, name, &wl_keyboard_interface, version);
}
void RegistryGlobalRemove(void *a, wl_registry *b, uint32_t c)
{
}

wl_registry_listener RegistryListener = {
    .global = RegistryGlobal,
    .global_remove = RegistryGlobalRemove};

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
Display CreateWayland()
{
  auto data = Display();

  data.Display = wl_display_connect(nullptr);
  if (data.Display == NULL)
  {
    perror("failed to connect to wayland display server");
    exit(EXIT_FAILURE);
  }

  //register variables
  const auto registry = wl_display_get_registry(data.Display);
  wl_registry_add_listener(registry, &RegistryListener, nullptr);
  wl_display_roundtrip(data.Display);
  wl_registry_destroy(registry);

  //setup structure
  data.Compositor = compositor;
  data.Shm = shm;
  data.Shell = shell;
  data.Seat = seat;
  data.Pointer = pointer;
  data.Keyboard = keyboard;

  return data;
}
void DestroyWayland(Display data)
{
  wl_keyboard_destroy(data.Keyboard);
  wl_pointer_destroy(data.Pointer);
  wl_seat_destroy(data.Seat);
  wl_shell_destroy(data.Shell);
  wl_shm_destroy(data.Shm);
  wl_compositor_destroy(data.Compositor);
  wl_display_disconnect(data.Display);
}
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga