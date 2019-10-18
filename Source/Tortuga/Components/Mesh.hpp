#ifndef _MESH_COMPONENT
#define _MESH_COMPONENT

#include <vector>
#include <cstring>
#include <glm/glm.hpp>

#include "../Core/Engine.hpp"

namespace Tortuga
{
namespace Components
{
struct Mesh : public Core::ECS::Component
{
  struct Index
  {
    uint32_t Position;
    uint32_t Texture;
    uint32_t Normal;
  };

  std::vector<glm::vec4> Positions;
  std::vector<glm::vec4> Textures;
  std::vector<glm::vec4> Normals;
  std::vector<Index> Indices;
};
} // namespace Components
} // namespace Tortuga

#endif