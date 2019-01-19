#include "Application.h"

namespace Tortuga
{
Application::Application()
{
    SDL_Init(SDL_INIT_EVERYTHING);
}

Application::~Application()
{
    delete _vulkan;
    delete _mainWindow;
    SDL_Quit();
}

void Application::Initialize()
{
    Console::Info("Tortuga Engine Started!");
    _mainWindow = new Window(this->ApplicationName.c_str(), 1024, 768);
    _vulkan = new Vulkan(_mainWindow, this->ApplicationName.c_str());
}

void Application::Run()
{
    SDL_Delay(1000);
}
} // namespace Tortuga