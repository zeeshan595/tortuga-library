#ifndef _TRANSFORMATION
#define _TRANSFORMATION

#include "Core.h"

namespace Tortuga
{
struct Transformation
{
  glm::vec3 Position = {0.0f, 0.0f, 0.0f};
  glm::vec4 Rotation = {0.0f, 0.0f, 0.0f, 0.0f};
  glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
};
}; // namespace Tortuga

#endif