#ifndef _MESH_COMPONENT
#define _MESH_COMPONENT

#include <vector>
#include <cstring>
#include <glm/glm.hpp>

#include "../Core/Engine.hpp"
#include "../Graphics/AcceleratedMesh.hpp"

namespace Tortuga
{
namespace Components
{
struct Mesh : public Core::ECS::Component
{
private:
  Graphics::AcceleratedMesh MeshObject;

public:
  bool IsDirty = false;

  Mesh()
  {
  }
  Mesh(Graphics::AcceleratedMesh mesh)
  {
    MeshObject = mesh;
    IsDirty = true;
  }

  const Graphics::AcceleratedMesh GetMesh() const
  {
    return MeshObject;
  }
};
} // namespace Components
} // namespace Tortuga

#endif