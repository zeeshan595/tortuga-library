#ifndef _ENVIRONMENT
#define _ENVIRONMENT

#include "Core.h"

namespace Tortuga
{
struct EntityData
{
};
struct Entity
{
  std::string Name;
  glm::vec3 Position;
  glm::vec4 Rotation;
  glm::vec3 Scale;
  std::vector<EntityData> Data;
};
struct Environment
{
  std::vector<Entity> entities;
};
Entity CreateEntity(
    std::string name,
    glm::vec3 position = glm::vec3(0, 0, 0),
    glm::vec4 rotation = glm::vec4(0, 0, 0, 0),
    glm::vec3 scale = glm::vec3(1, 1, 1));
glm::mat4 GetEntityTransformationMatrix(Entity data);
}; // namespace Tortuga

#endif