#include <Tortuga.h>

using namespace Tortuga;

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    auto scene = CreateEnvironment("Default");
    auto sphereObject = CreateEntity(scene);
    AddEntityData<MeshRenderer>(sphereObject);
    auto temp = GetEntityData<MeshRenderer>(sphereObject);

    auto systemController = CreateSystemController(scene);
    AddSystem<RenderingSystem>(systemController);

    //Main loop
    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event) != 0)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                isRunning = false;
                break;
            }
        }

        ProcessSystemUpdate(systemController);
    }

    DestroySystemController(systemController);
    DestroyEnvironment(scene);

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}