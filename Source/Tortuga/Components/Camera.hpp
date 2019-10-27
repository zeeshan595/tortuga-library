#ifndef _CAMERA_COMPONENT
#define _CAMERA_COMPONENT

#include <vector>
#include <cstring>
#include <glm/glm.hpp>

#include "../Core/Engine.hpp"

namespace Tortuga
{
namespace Components
{
struct Camera : public Core::ECS::Component
{
private:
  bool IsDirty = false;
  glm::vec4 Viewport = {0, 0, 1, 1};
  uint32_t ResolutionWidth = 1920;
  uint32_t ResolutionHeight = 1080;
  float FieldOfView = 45;
  float NearClipPlane = 0.001f;
  float FarClipPlane = 1000.0f;
  bool PresentToScreen = true;

public:
  glm::vec4 GetViewport()
  {
    return this->Viewport;
  }
  void SetViewport(glm::vec4 viewport)
  {
    this->Viewport = viewport;
  }
  uint32_t GetResolutionWidth()
  {
    return this->ResolutionWidth;
  }
  uint32_t GetResolutionHeight()
  {
    return this->ResolutionHeight;
  }
  void SetResolution(uint32_t width, uint32_t height)
  {
    this->ResolutionWidth = width;
    this->ResolutionHeight = height;
    IsDirty = true;
  }
  float GetFieldOfView()
  {
    return this->FieldOfView;
  }
  void SetFieldOfView(float f)
  {
    this->FieldOfView = f;
    IsDirty = true;
  }
  float GetNearClipPlane()
  {
    return this->NearClipPlane;
  }
  void SetNearClipPlay(float f)
  {
    this->NearClipPlane = f;
    IsDirty = true;
  }
  float GetFarClipPlane()
  {
    return this->FarClipPlane;
  }
  void SetFarClipPlane(float f)
  {
    this->FarClipPlane = f;
    IsDirty = true;
  }
  bool GetPresentToScreen()
  {
    return this->PresentToScreen;
  }
  void SetPresentToScreen(bool presentToScreen)
  {
    this->PresentToScreen = presentToScreen;
  }
  bool GetIsDirty()
  {
    return this->IsDirty;
  }
  void SetIsDirty(bool isDirty)
  {
    this->IsDirty = isDirty;
  }
};
} // namespace Components
} // namespace Tortuga

#endif