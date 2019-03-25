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
    auto ent = CreateEntity(env);

    auto m = MeshRenderer();
    m.message = "Hello World";
    ent->AddComponent<MeshRenderer>(m);
    auto data = ent->GetComponent<MeshRenderer>();

    auto sysController = CreateSystemController();
    AddSystem<TestingSystem>(sysController);
    ProcessSystems(sysController, env);
    DestroySystemController(sysController);
    DestroyEntity(env, ent);
    DestroyEnvironment(env);

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
    }

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}