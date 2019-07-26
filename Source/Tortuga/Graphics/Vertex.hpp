#ifndef _VERTEX
#define _VERTEX

#include <glm/glm.hpp>

namespace Tortuga
{
namespace Graphics
{
struct Vertex
{
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec4 Color;
};
} // namespace Graphics
} // namespace Tortuga

#endif