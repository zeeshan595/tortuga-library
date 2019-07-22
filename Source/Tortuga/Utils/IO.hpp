#ifndef _UTILS_INPUT_OUTPUT
#define _UTILS_INPUT_OUTPUT

#include "../Console.hpp"

#include <fstream>
#include <string>
#include <vector>

namespace Tortuga
{
namespace Utils
{
namespace IO
{
std::vector<char> GetFileContents(std::string filePath);
void SetFileContents(std::string filePath, std::vector<char> data);
void SetFileContents(std::string filePath, const char* data, uint32_t size);
} // namespace IO
} // namespace Utils
} // namespace Tortuga

#endif