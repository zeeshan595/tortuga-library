#ifndef _CAMERA_COMPONENT
#define _CAMERA_COMPONENT

#include <vector>
#include <cstring>
#include <glm/glm.hpp>

#include "../Core/Engine.hpp"
#include "../Graphics/RenderTarget.hpp"

namespace Tortuga
{
namespace Components
{
struct Camera : public Core::ECS::Component
{
  glm::vec4 Viewport;
  uint32_t ResolutionWidth;
  uint32_t ResolutionHeight;
  float FieldOfView;
  float NearClipPlane;
  float FarClipPlane;
  Graphics::RenderTarget::RenderTarget RenderTarget;
};
} // namespace Components
} // namespace Tortuga

#endif