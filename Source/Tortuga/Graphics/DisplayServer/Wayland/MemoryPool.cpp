#include "./MemoryPool.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
MemoryPool CreatePool(Display wayland, uint32_t size)
{
  auto data = MemoryPool();
  data.Size = size;
  data.Memory = (int8_t *)malloc(size);
  data.Pool = wl_shm_create_pool(wayland.Shm, 0, size);
  if (data.Pool == nullptr)
  {
    perror("failed to create memory pool");
    return data;
  }
  return data;
}
void DestroyPool(MemoryPool data)
{
  wl_shm_pool_destroy(data.Pool);
  delete data.Memory;
}
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga