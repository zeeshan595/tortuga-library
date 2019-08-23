#ifndef _UTILS_INPUT_OUTPUT
#define _UTILS_INPUT_OUTPUT

#include "../Console.hpp"
#include "../Graphics/Vertex.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <cstring>

namespace Tortuga
{
namespace Utils
{
namespace IO
{
struct ObjExport {
  std::vector<Graphics::Vertex> Vertices;
  std::vector<uint> Indices;
};

ObjExport LoadObjFile(std::string filePath);
std::vector<char> GetFileContents(std::string filePath);
void SetFileContents(std::string filePath, std::vector<char> data);
void SetFileContents(std::string filePath, const char* data, uint32_t size);
} // namespace IO
} // namespace Utils
} // namespace Tortuga

#endif