#ifndef _LIGHT
#define _LIGHT

#include <glm/glm.hpp>

namespace Tortuga
{
namespace Component
{
struct Light
{
  float Intensity;
  float Range;
};
} // namespace Component
} // namespace Tortuga

#endif