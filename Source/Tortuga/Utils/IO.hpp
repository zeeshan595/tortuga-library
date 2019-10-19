#ifndef _UTILS_INPUT_OUTPUT
#define _UTILS_INPUT_OUTPUT

#include <stb/stb_image.h>
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <cstring>

#include "../Core/Console.hpp"
#include "../Graphics/Image.hpp"

namespace Tortuga
{
namespace Utils
{
namespace IO
{
struct OBJ
{
  struct Index
  {
    uint32_t Position;
    uint32_t Texture;
    uint32_t Normal;
  };

  std::vector<glm::vec3> Positions;
  std::vector<glm::vec2> Textures;
  std::vector<glm::vec3> Normals;
  std::vector<Index> Indices;
};

OBJ LoadObjFile(std::string filePath);
Graphics::Image LoadImageFile(std::string filePath);
std::string GetFileContents(std::string filePath);
void SetFileContents(std::string filePath, std::string data);
void SetFileContents(std::string filePath, const char *data, uint32_t size);
} // namespace IO
} // namespace Utils
} // namespace Tortuga

#endif