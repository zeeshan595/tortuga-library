#include "./System.h"

namespace Tortuga
{
SystemController *CreateSystemController()
{
    return new SystemController();
}
void DestroySystemController(SystemController *controller)
{
    for (uint32_t i = 0; i < controller->SystemsVector.size(); i++)
    {
        controller->SystemsVector[i]->OnEnd();
        delete controller->SystemsVector[i];
    }
    controller->SystemsVector.clear();
    controller->Systems.clear();
    delete controller;
}
void ProcessSystems(SystemController *controller, Environment *env)
{
    for (uint32_t i = 0; i < controller->SystemsVector.size(); i++)
    {
        auto types = controller->SystemsVector[i]->GetRegisteredTypes();
        std::unordered_map<std::type_index, std::vector<ComponentData<std::any>>> componentData;
        for (uint32_t j = 0; j < types.size(); j++)
        {
            componentData[types[j]] = GetComponents(env, types[j]);
        }

        controller->SystemsVector[i]->UpdateComponentData(componentData);
        controller->SystemsVector[i]->OnUpdate();
    }
}
}; // namespace Tortuga