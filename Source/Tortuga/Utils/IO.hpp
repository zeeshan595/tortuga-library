#ifndef _UTILS_INPUT_OUTPUT
#define _UTILS_INPUT_OUTPUT

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <cstring>

#include "../Core/Console.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Graphics/Image.hpp"

namespace Tortuga
{
namespace Utils
{
namespace IO
{
struct ObjExport
{
  std::vector<glm::vec4> Vertices;
  std::vector<glm::vec2> Textures;
  std::vector<glm::vec4> Normals;
  std::vector<uint32_t> VertexIndices;
  std::vector<uint32_t> TextureIndices;
  std::vector<uint32_t> NormalIndices;
};

ObjExport LoadObjFile(std::string filePath);
Graphics::Image LoadImageFile(std::string filePath);
std::vector<char> GetFileContents(std::string filePath);
void SetFileContents(std::string filePath, std::vector<char> data);
void SetFileContents(std::string filePath, const char *data, uint32_t size);
} // namespace IO
} // namespace Utils
} // namespace Tortuga

#endif