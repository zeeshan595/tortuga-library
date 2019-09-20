#include "./Buffer.hpp"

#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
Buffer CreateBuffer(MemoryPool pool, uint32_t width, uint32_t height)
{
  auto data = Buffer();
  data.Width = width;
  data.Height = height;
  data.Pool = pool.Pool;

  data.Buffer = wl_shm_pool_create_buffer(pool.Pool, 0, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  if (data.Buffer == nullptr)
  {
    perror("failed to create wayland buffer");
    return data;
  }
  return data;
}
void DestroyBuffer(Buffer data)
{
  wl_buffer_destroy(data.Buffer);
}
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga