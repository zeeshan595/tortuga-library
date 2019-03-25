#include <Tortuga.h>

using namespace Tortuga;

struct Temporary
{
    std::string message;
};

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");
 
    auto env = CreateEnvironment();
    auto controller = CreateSystemController();
    AddSystem<RenderingSystem>(controller);

    //Main loop
    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                isRunning = false;
                break;
            }
            switch (event.type)
            {
            case SDL_KEYDOWN:
                Console::Info("Pressed!");
                break;
            }
        }

        ProcessSystems(controller, env);
    }
    DestroySystemController(controller);
    DestroyEnvironment(env);

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}