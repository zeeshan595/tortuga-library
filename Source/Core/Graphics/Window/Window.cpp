#include "Window.hpp"

using namespace Tortuga::Core::Graphics;

Window::Window(std::string title, uint32_t width, uint32_t height)
{
    this->_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (this->_window == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
        throw std::runtime_error("failed to create sdl window");
    }
}

Window::~Window()
{
    SDL_DestroyWindow(this->_window);
}

VkSurfaceKHR Window::CreateSurface(VkInstance instance)
{
    VkSurfaceKHR surface;
    if (!SDL_Vulkan_CreateSurface(this->_window, instance, &surface))
    {
        std::cout << SDL_GetError() << std::endl;
        throw std::runtime_error("failed to create vulkan surface");
    }
    return surface;
}

std::vector<const char *> Window::GetWindowExtensions()
{
    uint32_t count;
    if (!SDL_Vulkan_GetInstanceExtensions(this->_window, &count, nullptr))
    {
        std::cout << SDL_GetError() << std::endl;
        throw std::runtime_error("failed to get sdl window extensions count");
    }
    std::vector<const char *> extensions;
    if (!SDL_Vulkan_GetInstanceExtensions(this->_window, &count, extensions.data()))
    {
        std::cout << SDL_GetError() << std::endl;
        throw std::runtime_error("failed to get sdl window extensions");
    }
    //Extra extensions required
    extensions.push_back("VK_KHR_xlib_surface");
    return extensions;
}