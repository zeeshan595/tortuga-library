#ifndef _MESH_COMPONENT
#define _MESH_COMPONENT

#include <vector>
#include <cstring>
#include <glm/glm.hpp>

#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Core/Engine.hpp"

namespace Tortuga
{
namespace Components
{
struct Mesh : public Core::ECS::Component
{
  std::vector<glm::vec4> Vertices;
  std::vector<glm::vec2> Textures;
  std::vector<glm::vec4> Normals;
  std::vector<uint32_t> VertexIndices;
  std::vector<uint32_t> TextureIndices;
  std::vector<uint32_t> NormalIndices;
};
} // namespace Components
} // namespace Tortuga

#endif