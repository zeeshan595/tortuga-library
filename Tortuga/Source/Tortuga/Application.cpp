#include "Application.h"

namespace Tortuga
{
void Application::Initialize()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");
    _mainWindow = new Window(this->ApplicationName.c_str(), 1024, 768);
    _vulkan = new Vulkan(_mainWindow, this->ApplicationName.c_str());
    _swapchain = new Swapchain(_vulkan->GetDevices()[0], _mainWindow->GetWidth(), _mainWindow->GetHeight());
}

void Application::Destroy()
{
    delete _swapchain;
    delete _vulkan;
    delete _mainWindow;
    SDL_Quit();
}

void Application::Run()
{
    SDL_Delay(2000);
}
} // namespace Tortuga