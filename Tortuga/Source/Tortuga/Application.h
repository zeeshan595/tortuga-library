#ifndef _APPLICATION
#define _APPLICATION

#include "Core.h"
#include "Console.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan.h"
#include "Graphics/Swapchain.h"

namespace Tortuga
{
class Application
{
private:
  Window *_mainWindow;
  Vulkan *_vulkan;
  Swapchain *_swapchain;

protected:
  std::string ApplicationName = "My Application";

public:
  void Run();
  void Initialize();
  void Destroy();
};

//Will be defined in application
Application *CreateApplication();
}; // namespace Tortuga

#endif