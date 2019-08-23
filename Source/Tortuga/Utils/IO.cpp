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
  std::vector<uint> vertexIndices, textureIndices, normalIndices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_texture;
  std::vector<glm::vec3> temp_normals;

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
      temp_vertices.push_back(vertex);
    }
    else if (strcmp(lineHeader, "vt") == 0)
    {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      temp_texture.push_back(uv);
    }
    else if (strcmp(lineHeader, "vn") == 0)
    {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
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
      vertexIndices.push_back(vertexIndex[0] - 1);
      vertexIndices.push_back(vertexIndex[1] - 1);
      vertexIndices.push_back(vertexIndex[2] - 1);
      textureIndices.push_back(uvIndex[0] - 1);
      textureIndices.push_back(uvIndex[1] - 1);
      textureIndices.push_back(uvIndex[2] - 1);
      normalIndices.push_back(normalIndex[0] - 1);
      normalIndices.push_back(normalIndex[1] - 1);
      normalIndices.push_back(normalIndex[2] - 1);
    }
  }
  if (vertexIndices.size() != textureIndices.size() || textureIndices.size() != normalIndices.size())
    return {};

  std::vector<Graphics::Vertex> vertices(vertexIndices.size());
  std::vector<uint> indices;
  for (uint32_t i = 0; i < vertexIndices.size(); i++)
  {
    indices.push_back(i);
    vertices[i].Position = glm::vec4(
        temp_vertices[vertexIndices[i]].x,
        temp_vertices[vertexIndices[i]].y,
        temp_vertices[vertexIndices[i]].z,
        1.0f);
    vertices[i].Normal = glm::vec4(
        temp_normals[normalIndices[i]].x,
        temp_normals[normalIndices[i]].y,
        temp_normals[normalIndices[i]].z,
        1.0f);
  }

  return {vertices, indices};
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