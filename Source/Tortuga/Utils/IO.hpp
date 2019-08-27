#ifndef _UTILS_INPUT_OUTPUT
#define _UTILS_INPUT_OUTPUT

#include "../Console.hpp"

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
struct ObjExport
{
  std::vector<glm::vec4> Vertices;
  std::vector<glm::vec4> Normals;
  std::vector<glm::vec4> Textures;
  //std::vector<Graphics::Index> Indices;
};

ObjExport LoadObjFile(std::string filePath);
std::vector<char> GetFileContents(std::string filePath);
void SetFileContents(std::string filePath, std::vector<char> data);
void SetFileContents(std::string filePath, const char *data, uint32_t size);
} // namespace IO
} // namespace Utils
} // namespace Tortuga

#endif