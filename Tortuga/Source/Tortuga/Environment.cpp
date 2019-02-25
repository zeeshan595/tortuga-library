#include "Environment.h"

namespace Tortuga
{
Entity CreateEntity(
    std::string name,
    glm::vec3 position,
    glm::vec4 rotation,
    glm::vec3 scale)
{
  auto data = Entity();
  {
    data.Name = name;
    data.Position = position;
    data.Rotation = rotation;
    data.Scale = scale;
  }
  return data;
}
glm::mat4 GetEntityTransformationMatrix(Entity data)
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, data.Scale);
  model = glm::rotate(model, data.Rotation.w, glm::vec3(data.Rotation.x, data.Rotation.y, data.Rotation.z));
  model = glm::translate(model, data.Position);
  return model;
}
}; // namespace Tortuga
