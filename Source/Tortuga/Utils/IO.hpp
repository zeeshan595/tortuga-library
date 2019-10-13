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
#include "../Graphics/Image.hpp"
#include "../Graphics/AcceleratedMesh.hpp"

namespace Tortuga
{
namespace Utils
{
namespace IO
{
Graphics::AcceleratedMesh LoadObjFile(std::string filePath);
Graphics::Image LoadImageFile(std::string filePath);
std::string GetFileContents(std::string filePath);
void SetFileContents(std::string filePath, std::string data);
void SetFileContents(std::string filePath, const char *data, uint32_t size);
} // namespace IO
} // namespace Utils
} // namespace Tortuga

#endif