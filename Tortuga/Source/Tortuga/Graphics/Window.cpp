#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
Window::Window(const char *title, uint32_t width, uint32_t height)
{
    this->_width = width;
    this->_height = height;
    this->_window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_VULKAN);

    if (this->_window == nullptr)
    {
        Console::Fatal("Failed to create SDL window: {0}", SDL_GetError());
    }
}

Window::~Window()
{
    SDL_DestroyWindow(this->_window);
}

VkSurfaceKHR Window::CreateWindowSurface(VkInstance instance)
{
    VkSurfaceKHR surface;
    if (SDL_Vulkan_CreateSurface(this->_window, instance, &surface) == false)
    {
        Console::Fatal("Failed to get vulkan surface");
    }
    return surface;
}

std::vector<const char *> Window::GetVulkanInstanceExtensions()
{
    uint32_t count;
    if (SDL_Vulkan_GetInstanceExtensions(this->_window, &count, nullptr) == false)
    {
        Console::Fatal("Failed to get SDL window extensions count");
    }
    std::vector<const char *> extensions(count);
    if (SDL_Vulkan_GetInstanceExtensions(this->_window, &count, extensions.data()) == false)
    {
        Console::Fatal("Failed to get SDL window extensions list");
    }
    return extensions;
}
}; // namespace Graphics
}; // namespace Tortuga