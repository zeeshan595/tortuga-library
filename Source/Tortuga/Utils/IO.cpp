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
    Console::Error("Failed to open file: {0}", Console::Arguments()
                                                   << filePath);
    return {};
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();

  return buffer;
}
} // namespace IO
} // namespace Utils
} // namespace Tortuga