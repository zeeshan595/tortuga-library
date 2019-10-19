#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#include "./WaylandSurface.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Surface
{
class Wayland
{
public:
  wl_display *display = nullptr;
  wl_compositor *compositor = nullptr;
  wl_shell *shell = nullptr;
  wl_seat *seat = nullptr;
  wl_pointer *pointer = nullptr;
  wl_keyboard *keyboard = nullptr;
  wl_touch *touch = nullptr;

  Wayland() {}
  ~Wayland()
  {
    if (touch != nullptr)
      wl_touch_destroy(touch);
    if (keyboard != nullptr)
      wl_keyboard_destroy(keyboard);
    if (pointer != nullptr)
      wl_pointer_destroy(pointer);
    if (seat != nullptr)
      wl_seat_destroy(seat);
    if (shell != nullptr)
      wl_shell_destroy(shell);
    if (compositor != nullptr)
      wl_compositor_destroy(compositor);
    if (display != nullptr)
      wl_display_disconnect(display);
  }
};
auto _wayland = Wayland();

static void pointerHandleEnter(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy)
{
  fprintf(stderr, "Pointer entered surface %p at %d %d\n", surface, sx, sy);
}
static void pointerHandleLeave(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface)
{
  fprintf(stderr, "Pointer left surface %p\n", surface);
}

static void pointerHandleMotion(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
{
  printf("Pointer moved at %d %d\n", sx, sy);
}

static void pointerHandleButton(void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
  printf("Pointer button\n");
}

static void pointerHandleAxies(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
{
  printf("Pointer handle axis\n");
}

static const struct wl_pointer_listener pointerListener = {
    pointerHandleEnter,
    pointerHandleLeave,
    pointerHandleMotion,
    pointerHandleButton,
    pointerHandleAxies,
};

static void seatHandleCapabilities(void *data, struct wl_seat *seat, uint32_t capsInt)
{
  const auto caps = (wl_seat_capability)capsInt;
  if ((caps & WL_SEAT_CAPABILITY_POINTER) && _wayland.pointer == nullptr)
  {
    _wayland.pointer = wl_seat_get_pointer(seat);
    wl_pointer_add_listener(_wayland.pointer, &pointerListener, nullptr);
  }
  if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !_wayland.keyboard)
  {
  }
  if ((caps & WL_SEAT_CAPABILITY_TOUCH) && !_wayland.touch)
  {
  }
}

static const struct wl_seat_listener seatListener = {
    seatHandleCapabilities,
};

void globalRegistryHandler(void *data, struct wl_registry *registry, uint32_t id,
                           const char *interface, uint32_t version)
{
  if (strcmp(interface, "wl_compositor") == 0)
    _wayland.compositor = (wl_compositor *)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
  else if (strcmp(interface, "wl_shell") == 0)
    _wayland.shell = (wl_shell *)wl_registry_bind(registry, id, &wl_shell_interface, 1);
  else if (strcmp(interface, "wl_seat") == 0)
  {
    _wayland.seat = (wl_seat *)wl_registry_bind(registry, id, &wl_seat_interface, 1);
    wl_seat_add_listener(_wayland.seat, &seatListener, NULL);
  }
}

static void globalRegistryRemover(void *data, struct wl_registry *registry, uint32_t id)
{
}

static const struct wl_registry_listener registryRistener = {
    globalRegistryHandler,
    globalRegistryRemover};

WaylandSurface::WaylandSurface()
{
}
WaylandSurface::~WaylandSurface()
{
  for (const auto surf : ShellSurfaces)
  {
    if (surf != nullptr)
      wl_shell_surface_destroy(surf);
  }
  for (const auto surf : Surfaces)
  {
    if (surf != nullptr)
      wl_surface_destroy(surf);
  }

  ShellSurfaces.clear();
  Surfaces.clear();
}

void shellSurfacePing(void *data, struct wl_shell_surface *shell_surface, uint32_t serial)
{
  wl_shell_surface_pong(shell_surface, serial);
}

void shellSurfaceConfigure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height)
{
}

static const struct wl_shell_surface_listener shellSrfaceListener = {
    shellSurfacePing,
    shellSurfaceConfigure};

VkSurfaceKHR WaylandSurface::CreateSurface(VkInstance instance)
{
  if (_wayland.display == nullptr)
  {
    _wayland.display = wl_display_connect(nullptr);
    const auto registry = wl_display_get_registry(_wayland.display);
    wl_registry_add_listener(registry, &registryRistener, nullptr);
    Dispatch();
  }
  const auto surf = wl_compositor_create_surface(_wayland.compositor);
  const auto shellSurf = wl_shell_get_shell_surface(_wayland.shell, surf);
  wl_shell_surface_add_listener(shellSurf, &shellSrfaceListener, nullptr);
  wl_shell_surface_set_toplevel(shellSurf);
  Surfaces.push_back(surf);
  ShellSurfaces.push_back(shellSurf);

  auto createInfo = VkWaylandSurfaceCreateInfoKHR();
  {
    createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    createInfo.display = _wayland.display;
    createInfo.surface = surf;
  }
  auto data = VkSurfaceKHR();
  if (vkCreateWaylandSurfaceKHR(instance, &createInfo, nullptr, &data) != VK_SUCCESS)
  {
    Console::Error("Wayland: Failed to create vulkan surface");
  }

  return data;
}

void WaylandSurface::Dispatch()
{
  if (_wayland.display == nullptr)
    Console::Fatal("Wayland: disconnected from display");

  if (wl_display_dispatch(_wayland.display) == -1)
    Console::Error("Wayland: display error {0}", wl_display_get_error(_wayland.display));
  if (wl_display_roundtrip(_wayland.display) == -1)
    Console::Error("Wayland: display error {0}", wl_display_get_error(_wayland.display));
}
bool SurfaceInterface::HasPresentSupport(VkPhysicalDevice device, uint32_t familyIndex)
{
  const auto display = wl_display_connect(nullptr);
  if (display == nullptr)
    Console::Fatal("Wayland: cannot connect to display");
  return vkGetPhysicalDeviceWaylandPresentationSupportKHR(device, familyIndex, display) == VK_TRUE;
}
std::vector<const char *> SurfaceInterface::GetVulkanExtensions()
{
  return {VK_KHR_SURFACE_EXTENSION_NAME,
          VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME};
}
} // namespace Surface
} // namespace Graphics
} // namespace Tortuga
#endif