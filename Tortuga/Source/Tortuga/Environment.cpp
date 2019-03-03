#include "Environment.h"

namespace Tortuga
{
Environment CreateEnvironment(std::string name)
{
  return {name, {}};
}
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

void DestroyEntity(Environment &e, Entity *data)
{
  auto i = std::find(e.Entities.begin(), e.Entities.end(), data);
  e.Entities.erase(i);
  delete data;
}
void DestroyEnvironment(Environment data)
{
  for (uint32_t i = 0; i < data.Entities.size(); i++)
  {
    delete data.Entities[i];
  }
}
}; // namespace Tortuga
