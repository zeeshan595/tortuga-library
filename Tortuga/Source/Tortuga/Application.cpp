#include "Application.h"

namespace Tortuga
{
SDL_Window *window;

void InitializeEngine(const char *applicationName)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    window = SDL_CreateWindow(
        applicationName,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 768,
        SDL_WINDOW_VULKAN);

    auto vulkan = Graphics::VulkanAPI::CreateVulkanInstance();
    Graphics::VulkanAPI::DestroyVulkanInstance(vulkan);
}

void MainLoop()
{
    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event) != 0)
        {
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                isRunning = false;
            }
        }
    }
}

void DestroyEngine()
{
    SDL_DestroyWindow(window);

    //Console::Info("Shutting Down!");
    SDL_Quit();
}
} // namespace Tortuga