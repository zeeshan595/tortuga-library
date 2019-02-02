#ifndef _APPLICATION
#define _APPLICATION

#include "Core.h"
#include "Console.h"
#include "Graphics/VulkanAPI/Window.h"
#include "Graphics/VulkanAPI/Vulkan.h"
#include "Graphics/VulkanAPI/Swapchain.h"
#include "Graphics/VulkanAPI/Shader.h"
#include "Graphics/VulkanAPI/Pipeline.h"
#include "Graphics/VulkanAPI/FrameBuffer.h"
#include "Graphics/VulkanAPI/CommandPool.h"
#include "Graphics/VulkanAPI/CommandBuffer.h"
#include "Graphics/VulkanAPI/Buffer.h"
#include "Graphics/VulkanAPI/Renderer.h"

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
  RenderPass *_renderPass;
  Shader *_shader;
  Pipeline *_pipeline;
  std::vector<FrameBuffer *> _frameBuffers;
  Renderer *_renderer;
  CommandPool *_commandPool;
  CommandBuffer *_commandBuffer;
  CommandBuffer *_commandBuffer2;
  Buffer *_vertexBuffer;
  Buffer *_indexBuffer;
  Buffer *_vertexBuffer2;

  const std::vector<Vertex> vertices = {
      {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}},
      {{0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
      {{0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
      {{-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}};

  const std::vector<Vertex> vertices2 = {
      {{-0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
      {{1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
      {{1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
      {{0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}};

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