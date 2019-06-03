#include <Tortuga.h>

using namespace Tortuga;

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_VIDEO);

  auto vulkan = Graphics::CreateVulkanInstance();
  
  Graphics::DestroyVulkanInstance(vulkan);

  SDL_Quit();
  return 0;
}