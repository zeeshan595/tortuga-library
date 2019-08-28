#ifndef _LIGHT
#define _LIGHT

#include <glm/glm.hpp>

namespace Tortuga
{
namespace Component
{
enum LightType
{
  POINT,
  DIRECTIONAL
};
struct Light
{
  LightType Type;
  glm::vec4 Color;
  uint32_t Intensity;
  uint32_t Range;

  Light()
  {
    Type = LightType::POINT;
    Color = glm::vec4(1, 0, 0, 0);
    Intensity = 1.0f;
    Range = 10.0f;
  }
};
} // namespace Component
} // namespace Tortuga

#endif