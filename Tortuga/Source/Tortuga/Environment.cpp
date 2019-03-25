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
    env->Entities.push_back(e);
    return e;
}
void DestroyEnvironment(Environment *env)
{
    for (auto i = env->Entities.begin(); i < env->Entities.end(); i++)
    {
        delete (*i);
    }
    env->Entities.clear();
    delete env;
}
void DestroyEntity(Environment *env, Entity *entity)
{
    for (auto i = env->Entities.begin(); i < env->Entities.end(); i++)
    {
        if ((*i) == entity)
        {
            delete (*i);
            env->Entities.erase(i);
            break;
        }
    }
}

std::vector<ComponentData<std::any>> GetComponents(Environment *env, std::type_index typeInfo)
{
    std::vector<ComponentData<std::any>> data(env->Entities.size());
    for (uint32_t i = 0; i < env->Entities.size(); i++)
    {
        auto e = env->Entities[i];
        data[i] = {e, e->GetComponent(typeInfo)};
    }
    return data;
}
}; // namespace Tortuga