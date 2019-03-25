#ifndef _ENVIRONMENT
#define _ENVIRONMENT

#include "./Core.h"
#include "./Console.h"
#include "./Entity.h"

namespace Tortuga
{
struct Environment
{
    std::vector<Entity *> Entities;
};
template <typename T>
struct ComponentData
{
    Entity *Reference;
    T Data;
};

Environment *CreateEnvironment();
Entity *CreateEntity(Environment *env);
void DestroyEnvironment(Environment *env);
void DestroyEntity(Environment *env, Entity *entity);

std::vector<ComponentData<std::any>> GetComponents(Environment *env, std::type_index typeInfo);
}; // namespace Tortuga

#endif