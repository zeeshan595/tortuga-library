#ifndef _APPLICATION
#define _APPLICATION

#include "Core.h"
#include "Console.h"
#include "Graphics/Window.h"
#include "Graphics/Vulkan.h"
#include "Graphics/Swapchain.h"
#include "Graphics/Pipeline.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/CommandBuffer.h"

namespace Tortuga
{
class Application
{
private:
  std::string _applicationPath;
  std::string _applicationDir;
  Window *_mainWindow;
  Vulkan *_vulkan;
  Swapchain *_swapchain;
  Pipeline *_pipeline;
  std::vector<Framebuffer *> _frameBuffers;
  CommandBuffer *_commandBuffer;

protected:
  std::string ApplicationName = "My Application";

public:
  void Run();
  void Initialize(std::string path);
  void Destroy();

  std::string GetApplicationPath() { return _applicationPath; }
  std::string GetApplicationDirectory() { return _applicationDir; }
};

//Will be defined in application
Application *CreateApplication();
}; // namespace Tortuga

#endif