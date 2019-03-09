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

  static glm::mat4 GetEntityTransformationMatrix(Transformation data)
  {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, data.Scale);
    model = glm::rotate(model, data.Rotation.w, glm::vec3(data.Rotation.x, data.Rotation.y, data.Rotation.z));
    model = glm::translate(model, data.Position);
    return model;
  }
};
}; // namespace Tortuga

#endif