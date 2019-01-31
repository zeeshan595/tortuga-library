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
  std::vector<Buffer*> _vertexBuffer;
  std::vector<Buffer*> _indexBuffer;

  const std::vector<Vertex> vertices = {
      {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
      {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

  const std::vector<uint16_t> indices = {
      0, 1, 2, 2, 3, 0};

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