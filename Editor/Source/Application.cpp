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

    auto vulkan = Graphics::VulkanAPI::CreateVulkanInstance();
    auto devices = Graphics::VulkanAPI::CreateDevices(vulkan);
    auto window = Graphics::CreateWindow(vulkan, "Test application", 1024, 768, Graphics::WindowType::ResizeableWindowed);
    auto swapchain = Graphics::VulkanAPI::CreateSwapchain(devices[0], window.Surface, 1024, 768);

    auto vertexShader = Graphics::VulkanAPI::CreateShaderFromFile(devices[0], workingDirectory + "/Shaders/simple.vert.spv");
    auto fragmentShader = Graphics::VulkanAPI::CreateShaderFromFile(devices[0], workingDirectory + "/Shaders/simple.frag.spv");

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
            case SDL_WINDOWEVENT_RESIZED:

                break;
            }
        }
    }

    Graphics::VulkanAPI::DestroyShader(vertexShader);
    Graphics::VulkanAPI::DestroyShader(fragmentShader);
    Graphics::VulkanAPI::DestroySwapchain(swapchain);
    Graphics::DestroyWindow(window);
    Graphics::VulkanAPI::DestroyDevices(devices);
    Graphics::VulkanAPI::DestroyVulkanInstance(vulkan);

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}