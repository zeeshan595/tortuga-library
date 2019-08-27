#ifndef _CAMERA
#define _CAMERA

namespace Tortuga
{
  namespace Component
  {
    struct Camera
    {
      float FieldOfView;
      float aspectRatio;
      float nearClipPlane;
      float farClipPlane;
    };
  }
}

#endif