#ifndef _ENVIRONMENT
#define _ENVIRONMENT

#include "./Core.h"
#include "./Console.h"
#include "./Entity.h"

namespace Tortuga
{
struct Environment
{
    std::vector<Entity *> _entities;
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

template <typename T>
std::vector<ComponentData<T>> GetComponents(Environment *env)
{
    std::vector<ComponentData<T>> data(env->_entities.size());
    for (uint32_t i = 0; i < env->_entities.size(); i++)
    {
        auto e = env->_entities[i];
        data[i] = {e, e->GetComponent<T>()};
    }
    return data;
}
}; // namespace Tortuga

#endif