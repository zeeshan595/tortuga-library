#ifndef _TRANSFORM
#define _TRANSFORM

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

namespace Tortuga
{
namespace Component
{
struct Transform
{
private:
  bool IsDirty = false;
  glm::vec3 Position;
  glm::vec4 Rotation;
  glm::vec3 Scale;

public:
  Transform()
  {
    this->Position = glm::vec3(0, 0, 0);
    this->Rotation = glm::vec4(0, 0, 0, 1);
    this->Scale = glm::vec3(1, 1, 1);
    IsDirty = true;
  }

  glm::mat4 GetModelMatrix()
  {
    glm::mat4 transform = glm::mat4(1.0);
    transform = glm::translate(transform, Position);
    transform = glm::rotate(transform, Rotation.x, glm::vec3(Rotation.w, 0, 0));
    transform = glm::rotate(transform, Rotation.y, glm::vec3(0, Rotation.w, 0));
    transform = glm::rotate(transform, Rotation.z, glm::vec3(0, 0, Rotation.w));
    transform = glm::scale(transform, Scale);
    return transform;
  }

  glm::vec3 GetForward()
  {
    glm::mat4 transform = glm::mat4(1.0);
    transform = glm::rotate(transform, Rotation.x, glm::vec3(Rotation.w, 0, 0));
    transform = glm::rotate(transform, Rotation.y, glm::vec3(0, Rotation.w, 0));
    transform = glm::rotate(transform, Rotation.z, glm::vec3(0, 0, Rotation.w));
    return glm::normalize(glm::vec3(glm::inverse(transform)[2]));
  }

  bool GetIsDirty()
  {
    return IsDirty;
  }
  void SetPosition(glm::vec3 position)
  {
    Position = position;
    IsDirty = true;
  }
  void SetRotation(glm::vec4 rotation)
  {
    Rotation = rotation;
    IsDirty = true;
  }
  void SetScale(glm::vec3 scale)
  {
    Scale = scale;
    IsDirty = true;
  }
  glm::vec3 GetPosition()
  {
    return Position;
  }
  glm::vec4 GetRotation()
  {
    return Rotation;
  }
  glm::vec3 GetScale()
  {
    return Scale;
  }
};
} // namespace Component
} // namespace Tortuga

#endif