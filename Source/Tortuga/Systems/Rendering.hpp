#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#define MAXIMUM_NUM_OF_LIGHTS 10

#include <future>
#include <thread>
#include <bits/stdc++.h>

#include "../Core/ECS/Entity.hpp"
#include "../Core/ECS/System.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/DisplaySurface.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Core/Engine.hpp"
#include "../Graphics/RenderTarget.hpp"

#include "../Components/Mesh.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Camera.hpp"
#include "../Components/Light.hpp"

namespace Tortuga
{
namespace Systems
{
class Rendering : public Core::ECS::System
{
private:
  //vulkan instance and surface
  Graphics::Vulkan::Instance::Instance VulkanInstance;
  Graphics::DisplaySurface::DisplaySurface DisplaySurface;

  //general command pools
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool ComputeCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;

  //descriptor layouts
  std::vector<Graphics::Vulkan::DescriptorLayout::DescriptorLayout> DescriptorLayouts;

  //rendering
  std::vector<Graphics::Vulkan::Shader::Shader> Shaders;
  Graphics::Vulkan::RenderPass::RenderPass RenderPass;
  Graphics::Vulkan::Pipeline::Pipeline Pipeline;
  Graphics::Vulkan::Command::Command RenderCommand;

  //sync
  std::vector<Graphics::Vulkan::Semaphore::Semaphore> TransferSemaphore;
  std::vector<Graphics::Vulkan::Semaphore::Semaphore> RenderSemaphore;
  std::vector<Graphics::Vulkan::Fence::Fence> RenderFence;

  struct LightInfoStruct
  {
    glm::vec4 Color;
    int32_t Type;
    float Intensity;
    float Range;
  };

  struct MeshView : public Core::ECS::Component
  {
    bool IsStatic;
    bool IsTransformDirty;
    bool IsUpdated;
    uint32_t IndexCount;
    Graphics::Vulkan::Device::Device DeviceInUse;
    Graphics::Vulkan::Buffer::Buffer StagingVertexBuffer;
    Graphics::Vulkan::Buffer::Buffer VertexBuffer;
    Graphics::Vulkan::Buffer::Buffer StagingIndexBuffer;
    Graphics::Vulkan::Buffer::Buffer IndexBuffer;
    Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;
    Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
    Graphics::Vulkan::Command::Command RenderCommand;
    Graphics::Vulkan::Command::Command TransferCommand;
    Graphics::Vulkan::Buffer::Buffer StagingTransformBuffer;
    Graphics::Vulkan::Buffer::Buffer TransformBuffer;
    Graphics::Vulkan::Command::Command TransformTransferCommand;
    Graphics::Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
    Graphics::Vulkan::DescriptorSet::DescriptorSet TransformDescriptorSet;
    Graphics::Vulkan::Buffer::Buffer StagingLightsBuffer;
    Graphics::Vulkan::Buffer::Buffer LightsBuffer;
    Graphics::Vulkan::DescriptorSet::DescriptorSet LightsDescriptorSet;
    Graphics::Vulkan::Command::Command LightTransferCommand;

    void Setup(Graphics::Vulkan::Device::Device device, std::vector<Graphics::Vulkan::DescriptorLayout::DescriptorLayout> layouts);
    void OnDestroy();
    std::vector<Components::Light *> GetClosestLights();
    void UpdateLightsBuffer(std::vector<Components::Light *> lights);
  };

  struct CameraView : public Core::ECS::Component
  {
    Graphics::CameraRender::CameraRender Render;
    Graphics::Vulkan::DescriptorPool::DescriptorPool DescriptorPool;
    Graphics::Vulkan::DescriptorSet::DescriptorSet DescriptorSet;
    Graphics::Vulkan::Buffer::Buffer StagingCameraMatrix;
    Graphics::Vulkan::Buffer::Buffer CameraMatrix;
    Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
    Graphics::Vulkan::Command::Command TransferCommand;

    void Setup(Graphics::Vulkan::Device::Device device, Graphics::Vulkan::RenderPass::RenderPass renderPass, std::vector<Graphics::Vulkan::DescriptorLayout::DescriptorLayout> descriptorLayouts);
    void OnDestroy();
  };

  void SetupSemaphores(uint32_t size);

public:
  static uint32_t LightsPerMesh;

  Rendering();
  ~Rendering();

  void Update();
  void WaitForDevice();
};
} // namespace Systems
} // namespace Tortuga

#endif