#ifndef _TRANSFORM
#define _TRANSFORM

#include <glm/glm.hpp>

namespace Tortuga
{
namespace Component
{
struct Transform
{
  glm::vec3 Position;
  glm::vec4 Rotation;
  glm::vec3 Scale;
  glm::mat4x4 Matrix;
};
} // namespace Component
} // namespace Tortuga

#endif