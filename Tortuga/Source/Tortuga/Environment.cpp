#include "Environment.h"

namespace Tortuga
{
Environment *CreateEnvironment()
{
    return new Environment();
}
Entity *CreateEntity(Environment *env)
{
    auto e = new Entity();
    env->_entities.push_back(e);
    return e;
}
void DestroyEnvironment(Environment *env)
{
    for (auto i = env->_entities.begin(); i < env->_entities.end(); i++)
    {
        delete (*i);
    }
    env->_entities.clear();
    delete env;
}
void DestroyEntity(Environment *env, Entity *entity)
{
    for (auto i = env->_entities.begin(); i < env->_entities.end(); i++)
    {
        if ((*i) == entity)
        {
            delete (*i);
            env->_entities.erase(i);
            break;
        }
    }
}
}; // namespace Tortuga