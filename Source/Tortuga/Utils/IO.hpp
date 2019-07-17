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
} // namespace IO
} // namespace Utils
} // namespace Tortuga

#endif