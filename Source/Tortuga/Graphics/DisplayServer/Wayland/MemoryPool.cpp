#include "./MemoryPool.hpp"
#include "../../../Core/GUID.hpp"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

std::vector<std::string> memoryPoolPaths;

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
MemoryPool CreatePool(Display wayland, uint32_t maxWidth, uint32_t maxHeight)
{
  auto data = MemoryPool();
  data.FilePath = "/tmp/tortuga-wayland-session-" + Core::GUID::GenerateGUID(32) + ".bin";
  struct stat stat;
  uint32_t size = maxWidth * maxHeight * 4;

  unlink(data.FilePath.c_str());
  const auto file = creat(data.FilePath.c_str(), O_TMPFILE);
  std::vector<char> temp(size);
  write(file, temp.data(), size);
  if (fstat(file, &stat) != 0)
  {
    perror("failed to create wayland memory pool");
    return data;
  }

  data.fd = file;
  data.Size = stat.st_size;
  mmap(data.Memory, data.Size, PROT_READ, MAP_SHARED, data.fd, 0);
  if (data.Memory == MAP_FAILED)
  {
    perror("failed to map wayland memory pool");
    return data;
  }
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
  unlink(data.FilePath.c_str());
  wl_shm_pool_destroy(data.Pool);
}
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga