#include <Tortuga.h>

using namespace Tortuga;

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    auto scene = CreateEnvironment();
    auto sphereObject = CreateEntity(scene);
    AddEntityDataStructure<MeshRenderer>(sphereObject);

    auto controller = CreateSystemController();
    AddSystem<RenderingSystem>(controller);

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

        ProcessSystemController(controller);
    }

    DestroySystemController(controller);
    DestroyEnvironment(scene);

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}
