#include "Application.h"

namespace Tortuga
{
void Application::Initialize(std::string path)
{
    this->_applicationPath = path;
    this->_applicationDir = path.substr(0, path.find_last_of("/"));

    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    _mainWindow = new Window(this->ApplicationName.c_str(), 1024, 768);
    _vulkan = new Vulkan(_mainWindow, this->ApplicationName.c_str());
    _swapchain = new Swapchain(_vulkan->GetDevices()[0], _mainWindow->GetWidth(), _mainWindow->GetHeight());
    _pipeline = new Pipeline(_swapchain, _applicationDir + "/Shaders/simple.vert.spv", _applicationDir + "/Shaders/simple.frag.spv");
}

void Application::Destroy()
{
    delete _pipeline;
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