#include "./IO.hpp"

namespace Tortuga
{
namespace Utils
{
namespace IO
{
std::vector<char> GetFileContents(std::string filePath)
{
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);
  if (!file.is_open())
  {
    Console::Warning("Failed to open file: {0}", Console::Arguments() << filePath);
    return {};
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}
void SetFileContents(std::string filePath, std::vector<char> data)
{
  SetFileContents(filePath, data.data(), data.size());
}
void SetFileContents(std::string filePath, const char *data, uint32_t size)
{
  std::ofstream file(filePath, std::ios::ate | std::ios::binary);
  if (!file.is_open())
  {
    Console::Warning("Failed to open file: {0}", Console::Arguments() << filePath);
  }
  file.write(data, size);
  file.close();
}
} // namespace IO
} // namespace Utils
} // namespace Tortuga