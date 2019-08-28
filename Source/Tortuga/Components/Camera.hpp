#ifndef _CAMERA
#define _CAMERA

namespace Tortuga
{
  namespace Component
  {
    struct Camera
    {
      float FieldOfView;
      float AspectRatio;
      float NearClipPlane;
      float FarClipPlane;
      glm::vec2 ViewportOffset;
      glm::vec2 ViewportSize;

      Camera()
      {
        FieldOfView = 45.0f;
        AspectRatio = 1.777777778;
        NearClipPlane = 0.01f;
        FarClipPlane = 100.0f; 
        ViewportOffset = glm::vec2(0, 0);
        ViewportSize = glm::vec2(1, 1);
      }
    };
  }
}

#endif