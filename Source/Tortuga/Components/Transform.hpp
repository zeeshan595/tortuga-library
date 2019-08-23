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

  Transform()
  {
    this->Position = glm::vec3(0, 0, 0);
    this->Rotation = glm::vec4(0, 0, 0, 1);
    this->Scale = glm::vec3(1, 1, 1);
  }
};
} // namespace Component
} // namespace Tortuga

#endif