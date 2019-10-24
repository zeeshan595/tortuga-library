#ifndef _TRANSFORM_COMPONENT
#define _TRANSFORM_COMPONENT

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Engine.hpp"

namespace Tortuga
{
namespace Components
{
struct Transform : public Core::ECS::Component
{
  glm::vec3 Position;
  glm::vec4 Rotation;
  glm::vec3 Scale;

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
};
} // namespace Components
} // namespace Tortuga

#endif