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
    data->Position = position;
    data->Rotation = rotation;
    data->Scale = scale;
  }
  environment.Entities.push_back(data);
  return data;
}
glm::mat4 GetEntityTransformationMatrix(const Entity *data)
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, data->Scale);
  model = glm::rotate(model, data->Rotation.w, glm::vec3(data->Rotation.x, data->Rotation.y, data->Rotation.z));
  model = glm::translate(model, data->Position);
  return model;
}
}; // namespace Tortuga
