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
  glm::vec4 Viewport = {0, 0, 1, 1};
  uint32_t ResolutionWidth = 1920;
  uint32_t ResolutionHeight = 1080;
  float FieldOfView = 45;
  float NearClipPlane = 0.001f;
  float FarClipPlane = 1000.0f;
  Graphics::CameraRender::CameraRender Render;
  bool PresentToScreen = true;

  void OnDestroy()
  {
    Graphics::CameraRender::Destroy(Render);
  }
};
} // namespace Components
} // namespace Tortuga

#endif