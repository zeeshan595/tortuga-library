#ifndef _APPLICATION
#define _APPLICATION

#include "Core.h"
#include "Console.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan.h"

namespace Tortuga
{
class Application
{
private:
  Window *_mainWindow;
  Vulkan *_vulkan;

protected:
  std::string ApplicationName = "My Application";

public:
  Application();
  virtual ~Application();

  void Run();
  void Initialize();
};

//Will be defined in application
Application *CreateApplication();
}; // namespace Tortuga

#endif