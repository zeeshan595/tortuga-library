#include <Tortuga.h>

using namespace Tortuga;

std::string GetWorkingDirectory(std::string executablePath)
{
    uint32_t i = executablePath.find_last_of('/');
    return executablePath.substr(0, i);
}

int main(int argc, char **argv)
{
    std::string executablePath = argv[0];
    std::string workingDirectory = GetWorkingDirectory(executablePath);

    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    auto renderingEngine = Graphics::CreateRenderingEngine();
    auto window = Graphics::CreateWindow({renderingEngine.Devices[0], renderingEngine.Devices[0]},
                                         "Hello World",
                                         1024, 768,
                                         Graphics::WindowType::ResizeableWindowed);

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
    }

    Graphics::DestroyWindow(window);
    Graphics::DestroyRenderingEngine(renderingEngine);

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}