#include "Headers.hpp"

using namespace Tortuga::Core::Graphics;

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = Window("Hello World", 1024, 768);
    auto vulkan = Vulkan(window.GetWindowExtensions());
    window.CreateSurface(vulkan.GetInstance());
    
    SDL_Delay(3000);
    SDL_Quit();
    return 0;
}