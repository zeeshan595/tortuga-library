#ifndef _TESTING_SYSTEM
#define _TESTING_SYSTEM

#include "../Core.h"
#include "../Console.h"
#include "../System.h"

namespace Tortuga
{
class TestingSystem : public System
{
    void OnStart()
    {
        RegisterComponent<MeshRenderer>();
    }

    void OnUpdate()
    {
        Console::Info("Hello World");
        auto temp = GetData<MeshRenderer>();
        Console::Info(temp[0].Data.message);
    }
};
}; // namespace Tortuga

#endif