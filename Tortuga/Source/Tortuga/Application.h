#ifndef _APPLICATION
#define _APPLICATION

#include "Core.h"
#include "Console.h"
#include "Graphics/Window.h"
#include "Graphics/VulkanAPI/Vulkan.h"
#include "Graphics/VulkanAPI/Swapchain.h"
#include "Graphics/VulkanAPI/Shader.h"
#include "Graphics/VulkanAPI/Pipeline.h"
#include "Graphics/VulkanAPI/FrameBuffer.h"
#include "Graphics/VulkanAPI/CommandPool.h"
#include "Graphics/VulkanAPI/CommandBuffer.h"
#include "Graphics/VulkanAPI/Buffer.h"
#include "Graphics/VulkanAPI/Renderer.h"
#include "Graphics/VulkanAPI/DescriptorSetLayout.h"
#include "Graphics/VulkanAPI/DescriptorPool.h"
#include "Graphics/VulkanAPI/DescriptorSet.h"
#include "Graphics/VulkanAPI/UniformBufferObject.h"
#include "Graphics/VulkanAPI/PipelineLayout.h"
#include "Graphics/VulkanAPI/VulkanImage.h"

#include "AssetManager/Image.h"

namespace Tortuga
{
using namespace AssetManager;
using namespace Graphics;
using namespace Graphics::VulkanAPI;

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
  PipelineLayout *_pipelineLayout;
  Pipeline *_pipeline;
  FrameBuffer *_frameBuffer;
  Renderer *_renderer;
  CommandPool *_commandPool;
  CommandBuffer *_commandBuffer;
  Buffer *_vertexBuffer;
  Buffer *_indexBuffer;
  Buffer *_uniformBuffer;

  Image *_imageAsset;
  Buffer *_imageBuffer;
  VulkanImage *_imageView;

  DescriptorPool *_descriptorPool;
  std::vector<DescriptorSetLayout *> _descriptorSetLayouts;
  DescriptorSet *_descriptorSet;

  UniformBufferObject ubo = {};

  const std::vector<Vertex> vertices = {
      {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{+1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{+1.0f, +1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{-1.0f, +1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

  const std::vector<uint16_t> indices = {
      0, 1, 2, 2, 3, 0};

protected:
  std::string ApplicationName = "My Application";

public:
  void Initialize(std::string path);
  void Destroy();

  void Run();

  std::string GetApplicationPath() { return _applicationPath; }
  std::string GetApplicationDirectory() { return _applicationDir; }
};

//Will be defined in application
Application *CreateApplication();
}; // namespace Tortuga

#endif