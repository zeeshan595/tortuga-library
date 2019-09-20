#ifndef _WAYLAND_BUFFER
#define _WAYLAND_BUFFER

#include "./Display.hpp"
#include "./MemoryPool.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
struct Buffer
{
  wl_shm_pool *Pool;
  wl_buffer *Buffer;
  uint32_t Width;
  uint32_t Height;
};
Buffer CreateBuffer(MemoryPool pool, uint32_t width, uint32_t height);
void DestroyBuffer(Buffer data);
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga

#endif