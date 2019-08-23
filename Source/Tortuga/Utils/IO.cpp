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
  std::vector<uint> indices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_uvs;
  std::vector<glm::vec3> temp_normals;

  std::vector<glm::vec2> texture;
  std::vector<glm::vec3> normal;

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
      temp_uvs.push_back(uv);
    }
    else if (strcmp(lineHeader, "vn") == 0)
    {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    }
    else if (strcmp(lineHeader, "f") == 0)
    {
      if (texture.size() != temp_vertices.size())
        texture.resize(temp_vertices.size());
      if (normal.size() != temp_vertices.size())
        normal.resize(temp_vertices.size());

      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9)
      {
        printf("File can't be read by our simple parser : ( Try exporting with other options\n");
        return {};
      }
      indices.push_back(vertexIndex[0] - 1);
      indices.push_back(vertexIndex[1] - 1);
      indices.push_back(vertexIndex[2] - 1);
      texture.push_back(temp_uvs[uvIndex[0] - 1]);
      texture.push_back(temp_uvs[uvIndex[1] - 1]);
      texture.push_back(temp_uvs[uvIndex[2] - 1]);
      normal.push_back(temp_normals[normalIndex[0] - 1]);
      normal.push_back(temp_normals[normalIndex[1] - 1]);
      normal.push_back(temp_normals[normalIndex[2] - 1]);
    }
  }

  std::vector<Graphics::Vertex> vertices(temp_vertices.size());
  for (uint32_t i = 0; i < vertices.size(); i++)
  {
    vertices[i].Position = glm::vec4(temp_vertices[i].x, temp_vertices[i].y, temp_vertices[i].z, 1.0f);
    //vertices[i].Texture = glm::vec4(texture[i].x, texture[i].y, 0, 0);
    vertices[i].Normal = glm::vec4(normal[i].x, normal[i].y, normal[i].z, 1.0f);
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