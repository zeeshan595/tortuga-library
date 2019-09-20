#include "./MemoryPool.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

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

  struct stat stat;
  auto file = open("wayland-memory-pool.bin", O_RDWR);
  {
    if (file < 0)
    {
      file = creat("wayland-memory-pool.bin", O_RDWR);
      std::vector<char> temp(size);
      write(file, temp.data(), size);
    }
    if (fstat(file, &stat) != 0)
    {
      perror("failed to create wayland memory pool");
      return data;
    }
  }

  data.fd = file;
  data.Size = stat.st_size;
  data.Memory = (uint8_t *)mmap(0, data.Size, PROT_READ, MAP_SHARED, data.fd, 0);
  data.Pool = wl_shm_create_pool(wayland.Shm, data.fd, size);
  if (data.Pool == nullptr)
  {
    munmap(data.Memory, data.Size);
    perror("failed to create memory pool");
    return data;
  }
  return data;
}
void DestroyPool(MemoryPool data)
{
  munmap(data.Memory, data.Size);
  close(data.fd);
  unlink("wayland-memory-pool.bin");
  wl_shm_pool_destroy(data.Pool);
}
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga