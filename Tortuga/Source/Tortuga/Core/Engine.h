#ifndef _CORE_ENGINE
#define _CORE_ENGINE

#include "../Console.h"
#include "../Graphics/VulkanInstance.h"
#include "../Graphics/Window.h"
#include "./Environment.h"
#include "./System.h"
#include <SDL2/SDL.h>

namespace Tortuga {
namespace Core {
Environment Env;
Graphics::VulkanInstance Vulkan;
Graphics::Window Window;
void CreateEngine() {
  Env = {{}};
  SDL_Init(SDL_INIT_EVERYTHING);
  Vulkan = Graphics::CreateVulkanInstance();
}
void DestroyEngine() {
  Graphics::DestroyVulkanInstance(Vulkan);
  SDL_Quit();
}
void EngineMainLoop() {
  for (std::pair<std::type_index, System *> element : Systems) {
    element.second->Start();
    auto registeredComponents = element.second->RegisteredComponents;
    for (uint32_t i = 0; i < registeredComponents.size(); i++) {
      auto typeIndex = std::type_index(typeid(registeredComponents));
      element.second->Components[typeIndex] = GetComponents(Env, typeIndex);
    }
  }
  while (true) {
    for (std::pair<std::type_index, System *> element : Systems) {
      element.second->Update();

      auto registeredComponents = element.second->RegisteredComponents;
      for (uint32_t i = 0; i < registeredComponents.size(); i++) {
        auto typeIndex = std::type_index(typeid(registeredComponents));
        for (uint32_t j = 0; j < element.second->Components[typeIndex].size();
             j++) {
          element.second->Components[typeIndex][j]
              .entity->Components[typeIndex] =
              element.second->Components[typeIndex][j].Component;
        }
      }
    }
  }
  for (std::pair<std::type_index, System *> element : Systems) {
    element.second->End();
  }
}
} // namespace Core
} // namespace Tortuga

#endif