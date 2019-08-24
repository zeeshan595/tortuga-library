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
ObjExport LoadObjFile(std::string filePath)
{
  ObjExport exporter = {};

  FILE *file = fopen(filePath.c_str(), "r");
  if (file == NULL)
  {
    printf("Impossible to open the file !\n");
    return {};
  }
  while (true)
  {
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break; // EOF = End Of File. Quit the loop.

    // else : parse lineHeader
    if (strcmp(lineHeader, "v") == 0)
    {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      exporter.Vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));
    }
    else if (strcmp(lineHeader, "vt") == 0)
    {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      exporter.Textures.push_back(glm::vec4(uv.x, uv.y, 0, 1));
    }
    else if (strcmp(lineHeader, "vn") == 0)
    {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      exporter.Normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 1));
    }
    else if (strcmp(lineHeader, "f") == 0)
    {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9)
      {
        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
        return {};
      }
      Graphics::Index index1, index2, index3;
      index1.Vertex = vertexIndex[0] - 1;
      index2.Vertex = vertexIndex[1] - 1;
      index3.Vertex = vertexIndex[2] - 1;
      index1.Texture = uvIndex[0] - 1;
      index2.Texture = uvIndex[1] - 1;
      index3.Texture = uvIndex[2] - 1;
      index1.Normal = normalIndex[0] - 1;
      index2.Normal = normalIndex[1] - 1;
      index3.Normal = normalIndex[2] - 1;
      exporter.Indices.push_back(index1);
      exporter.Indices.push_back(index2);
      exporter.Indices.push_back(index3);
    }
  }
  return exporter;
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