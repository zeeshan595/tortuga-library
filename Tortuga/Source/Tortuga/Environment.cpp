#include "Environment.h"

namespace Tortuga
{
Entity *CreateEntity(
    Environment &environment,
    const char *name,
    glm::vec3 position,
    glm::vec4 rotation,
    glm::vec3 scale)
{
  auto data = new Entity();
  {
    data->Name = name;
    data->Transform.Position = position;
    data->Transform.Rotation = rotation;
    data->Transform.Scale = scale;
  }
  environment.Entities.push_back(data);
  return data;
}
glm::mat4 GetEntityTransformationMatrix(const Entity *data)
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, data->Transform.Scale);
  model = glm::rotate(model, data->Transform.Rotation.w, glm::vec3(data->Transform.Rotation.x, data->Transform.Rotation.y, data->Transform.Rotation.z));
  model = glm::translate(model, data->Transform.Position);
  return model;
}
}; // namespace Tortuga
