#ifndef _ENVIRONMENT
#define _ENVIRONMENT

#include "Core.h"
#include "Console.h"
#include "Transformation.h"

namespace Tortuga
{
struct EntityData
{
};
struct Entity
{
  std::string Name;
  Transformation Transform;
  std::vector<volatile EntityData *> Data;
};
struct Environment
{
  std::string Name;
  std::vector<Entity *> Entities;
};
Entity *CreateEntity(
    Environment &environment,
    const char *name = "New Entity",
    glm::vec3 position = glm::vec3(0, 0, 0),
    glm::vec4 rotation = glm::vec4(0, 0, 0, 0),
    glm::vec3 scale = glm::vec3(1, 1, 1));
glm::mat4 GetEntityTransformationMatrix(Entity data);

template <typename T>
void AddEntityData(Entity *e)
{
  auto temp = dynamic_cast<EntityData *>(new T());
  e->Data.push_back(temp);
}
}; // namespace Tortuga

#endif