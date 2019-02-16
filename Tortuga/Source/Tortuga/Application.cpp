#include "Application.h"

namespace Tortuga
{
Graphics::VulkanAPI::VulkanData vulkan;
std::vector<Graphics::VulkanAPI::DeviceData> devices;
Graphics::WindowData window;
Graphics::VulkanAPI::SwapchainData swapchain;

void InitializeEngine(const char *applicationName)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    vulkan = Graphics::VulkanAPI::CreateVulkanInstance();
    devices = Graphics::VulkanAPI::CreateDevices(vulkan);
    window = Graphics::CreateWindow(vulkan, applicationName, 1024, 768, Graphics::WindowType::ResizeableWindowed);
    swapchain = Graphics::VulkanAPI::CreateSwapchain(devices[0], window.Surface, 1024, 768);
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
    Graphics::VulkanAPI::DestroySwapchain(swapchain);
    Graphics::DestroyWindow(window);
    Graphics::VulkanAPI::DestroyDevices(devices);
    Graphics::VulkanAPI::DestroyVulkanInstance(vulkan);

    Console::Info("Shutting Down!");
    SDL_Quit();
}
} // namespace Tortuga