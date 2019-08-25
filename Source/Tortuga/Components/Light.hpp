#ifndef _LIGHT
#define _LIGHT

#include <glm/glm.hpp>

namespace Tortuga
{
namespace Component
{
enum LightType
{
  Point = 0,
  Directional = 1
};
struct Light
{
  LightType Type;
  float Intensity;
  float Range;

  Light()
  {
    this->Type = LightType::Point;
    this->Intensity = 1.0f;
    this->Range = 10.0f;
  }
};
} // namespace Component
} // namespace Tortuga

#endif