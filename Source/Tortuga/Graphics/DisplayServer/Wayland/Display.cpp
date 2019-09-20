#include "./Display.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

wl_compositor *compositor = nullptr;
wl_shm *shm = nullptr;
wl_shell *shell = nullptr;
wl_seat *seat = nullptr;
wl_pointer *pointer = nullptr;
wl_keyboard *keyboard = nullptr;
wl_touch *touch = nullptr;

void RegistryGlobal(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
  if (strcmp(interface, wl_compositor_interface.name) == 0)
    compositor = (wl_compositor *)wl_registry_bind(registry, name, &wl_compositor_interface, min(version, 4));
  else if (strcmp(interface, wl_shm_interface.name) == 0)
    shm = (wl_shm *)wl_registry_bind(registry, name, &wl_shm_interface, min(version, 1));
  else if (strcmp(interface, wl_shell_interface.name) == 0)
    shell = (wl_shell *)wl_registry_bind(registry, name, &wl_shell_interface, min(version, 1));
  else if (strcmp(interface, wl_seat_interface.name) == 0)
  {
    seat = (wl_seat *)wl_registry_bind(registry, name, &wl_seat_interface, min(version, 2));
    pointer = wl_seat_get_pointer(seat);
    keyboard = wl_seat_get_keyboard(seat);
    touch = wl_seat_get_touch(seat);
    //wl_pointer_add_listener(pointer, &pointer_listener, NULL);
  }
}
void RegistryGlobalRemove(void *a, wl_registry *b, uint32_t c)
{
}

wl_registry_listener RegistryListener = {
    .global = RegistryGlobal,
    .global_remove = RegistryGlobalRemove};

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
  data.Touch = touch;

  return data;
}
void DestroyWayland(Display data)
{
  wl_touch_destroy(data.Touch);
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