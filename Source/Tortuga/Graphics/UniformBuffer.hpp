#ifndef _GRAPHICS_UNIFORM_BUFFER
#define _GRAPHICS_UNIFORM_BUFFER

#include <glm/glm.hpp>

namespace Tortuga
{
namespace Graphics
{
struct UniformBufferObject
{
  glm::mat4 Model;
  glm::mat4 View;
  glm::mat4 Projection;
};
} // namespace Graphics
} // namespace Tortuga

#endif