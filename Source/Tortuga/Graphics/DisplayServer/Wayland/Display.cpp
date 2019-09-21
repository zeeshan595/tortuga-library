#include "./Display.hpp"

#include <future>

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

void PointerEnter(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface, wl_fixed_t x, wl_fixed_t y)
{
}

void PointerLeave(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface)
{
}

void PointerMotion(void *data, wl_pointer *pointer, uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
  //std::cout << x << std::endl;
}

void PointerButton(void *data, wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
}

void PointerAxis(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
{
}

wl_pointer_listener PointerListener = {
    .enter = PointerEnter,
    .leave = PointerLeave,
    .motion = PointerMotion,
    .button = PointerButton,
    .axis = PointerAxis,
};

void KeyMap(void *data, wl_keyboard *keyboard, uint32_t format, int32_t fd, uint32_t size)
{
}

void KeyboardFocusEnter(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface, wl_array *keys)
{
}
void KeyboardFocusLeave(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface)
{
}

void KeyboardEvent(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
  std::cout << key << std::endl;
}

void ModifiersEvent(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t modsDepressed, uint32_t modsLatched, uint32_t modsLocked, uint32_t group)
{
}

void RepeatEvent(void *data, wl_keyboard *keyboard, int32_t rate, int32_t delay)
{
}

wl_keyboard_listener KeyboardListener = {
    .keymap = KeyMap,
    .enter = KeyboardFocusEnter,
    .leave = KeyboardFocusLeave,
    .key = KeyboardEvent,
    .modifiers = ModifiersEvent,
    .repeat_info = RepeatEvent,
};

void TouchStart(void *data, wl_touch *touch, uint32_t serial, uint32_t time, wl_surface *surface, int32_t id, wl_fixed_t x, wl_fixed_t y)
{
}
void TouchEnd(void *data, wl_touch *touch, uint32_t serial, uint32_t time, int32_t id)
{
}
void TouchMotion(void *data, wl_touch *touch, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y)
{
}
void TouchFrame(void *data, struct wl_touch *wl_touch)
{
}
void TouchCancel(void *data, wl_touch *touch)
{
}
void TouchShape(void *data, wl_touch *touch, int32_t id, wl_fixed_t major, wl_fixed_t minor)
{
}

void Orientation(void *data, wl_touch *touch, int32_t id, wl_fixed_t orientation)
{
}

wl_touch_listener TouchListener = {
    .down = TouchStart,
    .up = TouchEnd,
    .motion = TouchMotion,
    .frame = TouchFrame,
    .cancel = TouchCancel,
    .shape = TouchShape,
    .orientation = Orientation,
};

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
    wl_pointer_add_listener(pointer, &PointerListener, nullptr);
    wl_keyboard_add_listener(keyboard, &KeyboardListener, nullptr);
    wl_touch_add_listener(touch, &TouchListener, nullptr);
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
  if (data.Display == nullptr)
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

void Dispatch(Display data)
{
  while (wl_display_prepare_read(data.Display) != 0)
    wl_display_dispatch_pending(data.Display);

  wl_display_read_events(data.Display);
}
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga