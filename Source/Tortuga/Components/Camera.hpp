#ifndef _CAMERA
#define _CAMERA

#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "./Transform.hpp"

namespace Tortuga
{
namespace Component
{
struct Camera
{
private:
  Graphics::Vulkan::Device::Device device;
  bool IsDirty = false;
  float FieldOfView;
  float AspectRatio;
  float NearClipPlane;
  float FarClipPlane;
  glm::vec2 ViewportOffset;
  glm::vec2 ViewportSize;

public:
  bool IsEnabled;
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::Command::Command TransferCommand;
  Graphics::Vulkan::DescriptorPool::DescriptorPool CameraDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet CameraDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer StagingCameraBuffer;
  Graphics::Vulkan::Buffer::Buffer CameraBuffer;

  Camera()
  {
    IsEnabled = true;
    FieldOfView = 45.0f;
    AspectRatio = 1.777777778;
    NearClipPlane = 0.01f;
    FarClipPlane = 100.0f;
    ViewportOffset = glm::vec2(0, 0);
    ViewportSize = glm::vec2(1, 1);

    device = Core::Engine::GetMainDevice();

    TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
    TransferCommand = Graphics::Vulkan::Command::Create(device, TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);

    CameraDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {Core::Engine::GetVertexUniformBufferLayout()});
    CameraDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, CameraDescriptorPool, {Core::Engine::GetVertexUniformBufferLayout()});
    StagingCameraBuffer = Graphics::Vulkan::Buffer::CreateHostSrc(device, sizeof(glm::mat4) * 2);
    CameraBuffer = Graphics::Vulkan::Buffer::Create(device, sizeof(glm::mat4) * 2, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    Graphics::Vulkan::DescriptorSet::UpdateDescriptorSets(CameraDescriptorSet, {CameraBuffer});
    IsDirty = true;
  }
  ~Camera()
  {
    Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);

    Graphics::Vulkan::DescriptorPool::Destroy(CameraDescriptorPool);
    Graphics::Vulkan::Buffer::Destroy(StagingCameraBuffer);
    Graphics::Vulkan::Buffer::Destroy(CameraBuffer);
  }

  void UpdateBuffers(Transform *transform)
  {
    if (!IsDirty && !transform->GetIsDirty())
      return;

    std::vector<glm::mat4> cameraMatrices(2);
    {
      //view
      cameraMatrices[0] = glm::mat4(1.0);
      if (transform != nullptr)
        cameraMatrices[0] = glm::inverse(transform->GetModelMatrix());

      //projection
      cameraMatrices[1] = glm::perspective(FieldOfView, AspectRatio, NearClipPlane, FarClipPlane);
      cameraMatrices[1][1][1] *= -1;
    }
    Graphics::Vulkan::Buffer::SetData(StagingCameraBuffer, cameraMatrices.data(), sizeof(glm::mat4) * 2);
    Graphics::Vulkan::Command::CopyBuffer(TransferCommand, StagingCameraBuffer, CameraBuffer);
    IsDirty = false;
  }

  void SetFieldOfView(float f)
  {
    IsDirty = true;
    FieldOfView = f;
  }
  void SetAspectRatio(float f)
  {
    IsDirty = true;
    AspectRatio = f;
  }
  void SetNearClipPlane(float f)
  {
    IsDirty = true;
    NearClipPlane = f;
  }
  void SetFarClipPlane(float f)
  {
    IsDirty = true;
    FarClipPlane = f;
  }
  void SetViewportOffset(glm::vec2 offset)
  {
    ViewportOffset = offset;
  }
  void SetViewportSize(glm::vec2 size)
  {
    ViewportSize = size;
  }
  float GetFieldOfView(float f)
  {
    return FieldOfView;
  }
  float GetAspectRatio(float f)
  {
    return AspectRatio;
  }
  float GetNearClipPlane(float f)
  {
    return NearClipPlane;
  }
  float GetFarClipPlane(float f)
  {
    return FarClipPlane;
  }
  glm::vec2 GetViewportOffset()
  {
    return ViewportOffset;
  }
  glm::vec2 GetViewportSize()
  {
    return ViewportSize;
  }
};
} // namespace Component
} // namespace Tortuga

#endif