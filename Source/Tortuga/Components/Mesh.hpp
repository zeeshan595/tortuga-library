#ifndef _MESH_COMPONENT
#define _MESH_COMPONENT

#include <vector>
#include <cstring>
#include <glm/glm.hpp>

#include "../Core/Engine.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Utils/IO.hpp"

namespace Tortuga
{
namespace Components
{
struct Mesh : public Core::ECS::Component
{
  bool IsStatic = false;
  std::vector<Graphics::Vertex> Vertices;
  std::vector<uint32_t> Indices;
  bool IsVerticesDirty = false;
  bool IsIndicesDirty = false;

  void SetVertices(std::vector<Graphics::Vertex> vertices)
  {
    this->Vertices = vertices;
    this->IsVerticesDirty = true;
  }

  void SetIndices(std::vector<uint32_t> indices)
  {
    this->Indices = indices;
    this->IsIndicesDirty = true;
  }

  Mesh()
  {
  }
  Mesh(Utils::IO::OBJ obj)
  {
    IsVerticesDirty = true;
    IsIndicesDirty = true;
    Vertices.resize(obj.Positions.size());
    Indices.resize(obj.Indices.size());
    for (uint32_t i = 0; i < obj.Indices.size(); i++)
    {
      Indices[i] = obj.Indices[i].Position;
      Vertices[obj.Indices[i].Position].Position = obj.Positions[obj.Indices[i].Position];
      Vertices[obj.Indices[i].Position].Texture = obj.Textures[obj.Indices[i].Texture];
      Vertices[obj.Indices[i].Position].Normal = obj.Normals[obj.Indices[i].Normal];
    }
  }
};
} // namespace Components
} // namespace Tortuga

#endif