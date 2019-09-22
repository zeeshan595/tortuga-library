#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "../Graphics/Vulkan/Device.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vulkan/Image.hpp"
#include "../Graphics/Vulkan/ImageView.hpp"
#include "../Graphics/Vulkan/Sampler.hpp"
#include "../Graphics/Vertex.hpp"
#include "../Core/Entity.hpp"

#include "./Light.hpp"
#include "./Camera.hpp"
#include "./Material.hpp"

namespace Tortuga
{
namespace Component
{
struct Mesh
{
private:
  bool IsDirty = false;
  Graphics::Vulkan::Device::Device device;
  std::vector<Core::Entity::Entity *> Lights;
  std::vector<Graphics::Vertex> Vertices;
  std::vector<uint32_t> Indices;

  struct LightInfo
  {
    glm::vec4 Position;
    glm::vec4 Forward;
    glm::vec4 Color;
    uint Type;
    float Intensity;
    float Range;

    LightInfo()
    {
      Position = glm::vec4(0, 0, 0, 1);
      Forward = glm::vec4(0, 0, 1, 1);
    }
  };

  std::vector<LightInfo> GetLightInfos()
  {
    std::vector<LightInfo> lightInfos;
    for (uint32_t i = 0; i < Lights.size(); i++)
    {
      if (lightInfos.size() >= Component::Light::MAX_LIGHT_NUM)
        break;

      const auto lightEntity = Lights[i];
      const auto light = lightEntity->GetComponent<Component::Light>();
      if (light == nullptr)
        continue;
      if (light->IsEnabled == false)
        continue;

      LightInfo info = {};
      {
        info.Color = light->Color;
        info.Intensity = light->Intensity;
        info.Range = light->Range;
        info.Type = light->Type;
        const auto transform = lightEntity->GetComponent<Component::Transform>();
        if (transform != nullptr)
        {
          info.Position = glm::vec4(transform->GetPosition(), 1.0);
          info.Forward = glm::vec4(transform->GetForward(), 1.0);
        }
      }
      lightInfos.push_back(info);
    }
    return lightInfos;
  }

public:
  //gpu commands
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool GraphicsProcessCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool RenderCommandPool;
  Graphics::Vulkan::Command::Command TransferCommand;
  Graphics::Vulkan::Command::Command GraphicsProcessCommand;
  Graphics::Vulkan::Command::Command RenderCommand;
  //mesh information
  Graphics::Vulkan::Buffer::Buffer StagingVertexBuffer;
  Graphics::Vulkan::Buffer::Buffer VertexBuffer;
  Graphics::Vulkan::Buffer::Buffer StagingIndexBuffer;
  Graphics::Vulkan::Buffer::Buffer IndexBuffer;
  //transform buffer
  Graphics::Vulkan::DescriptorPool::DescriptorPool TransformDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet TransformDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer StagingTransformBuffer;
  Graphics::Vulkan::Buffer::Buffer TransformBuffer;
  //light information
  Graphics::Vulkan::DescriptorPool::DescriptorPool LightDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet LightDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer StagingLightBuffer;
  Graphics::Vulkan::Buffer::Buffer LightBuffer;
  //material information
  Graphics::Vulkan::DescriptorPool::DescriptorPool MaterialDescriptorPool;
  //albedo
  Graphics::Vulkan::DescriptorSet::DescriptorSet AlbedoDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer StagingAlbedoImage;
  Graphics::Vulkan::Image::Image AlbedoImage;
  Graphics::Vulkan::ImageView::ImageView AlbedoImageView;
  Graphics::Vulkan::Sampler::Sampler AlbedoImageSampler;

  Mesh()
  {
    IsDirty = true;
    device = Core::Engine::GetMainDevice();

    TransferCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
    TransferCommand = Graphics::Vulkan::Command::Create(device, TransferCommandPool, Graphics::Vulkan::Command::PRIMARY);

    GraphicsProcessCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
    GraphicsProcessCommand = Graphics::Vulkan::Command::Create(device, GraphicsProcessCommandPool, Graphics::Vulkan::Command::PRIMARY);

    RenderCommandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Graphics.Index);
    RenderCommand = Graphics::Vulkan::Command::Create(device, RenderCommandPool, Graphics::Vulkan::Command::SECONDARY);

    StagingVertexBuffer = Graphics::Vulkan::Buffer::CreateHostSrc(device, sizeof(uint32_t));
    VertexBuffer = Graphics::Vulkan::Buffer::Create(device, sizeof(uint32_t), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    StagingIndexBuffer = Graphics::Vulkan::Buffer::CreateHostSrc(device, sizeof(uint32_t));
    IndexBuffer = Graphics::Vulkan::Buffer::Create(device, sizeof(uint32_t), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    TransformDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {Core::Engine::GetVertexUniformBufferLayout()});
    TransformDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, TransformDescriptorPool, Core::Engine::GetVertexUniformBufferLayout());
    StagingTransformBuffer = Graphics::Vulkan::Buffer::CreateHostSrc(device, sizeof(glm::mat4));
    TransformBuffer = Graphics::Vulkan::Buffer::Create(device, sizeof(glm::mat4), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    Graphics::Vulkan::DescriptorSet::UpdateDescriptorSets(TransformDescriptorSet, {TransformBuffer});

    //lights
    const auto lightInfos = GetLightInfos();
    LightDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {Core::Engine::GetVertexAndFragmentUniformBufferLayout()});
    LightDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, LightDescriptorPool, Core::Engine::GetVertexAndFragmentUniformBufferLayout());
    StagingLightBuffer = Graphics::Vulkan::Buffer::CreateHostSrc(device, sizeof(glm::vec4) + (sizeof(LightInfo) * Component::Light::MAX_LIGHT_NUM));
    LightBuffer = Graphics::Vulkan::Buffer::Create(device, sizeof(glm::vec4) + (sizeof(LightInfo) * Component::Light::MAX_LIGHT_NUM), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    uint32_t lightInfoSize = lightInfos.size();
    Graphics::Vulkan::Buffer::SetData(StagingLightBuffer, &lightInfoSize, StagingLightBuffer.Size);
    if (lightInfos.size() > 0)
      Graphics::Vulkan::Buffer::SetData(StagingLightBuffer, lightInfos.data(), sizeof(lightInfos[0]) * lightInfos.size(), sizeof(glm::vec4));
    Graphics::Vulkan::DescriptorSet::UpdateDescriptorSets(LightDescriptorSet, {LightBuffer});

    //material
    MaterialDescriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, {Core::Engine::GetFragmentCombinedImageSampleLayout()});
    //albedo
    AlbedoDescriptorSet = Graphics::Vulkan::DescriptorSet::Create(device, MaterialDescriptorPool, Core::Engine::GetFragmentCombinedImageSampleLayout());
    StagingAlbedoImage = Graphics::Vulkan::Buffer::CreateHostSrc(device, 4);
    AlbedoImage = Graphics::Vulkan::Image::Create(device, 1, 1, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
    AlbedoImageView = Graphics::Vulkan::ImageView::Create(device, AlbedoImage, VK_IMAGE_ASPECT_COLOR_BIT);
    AlbedoImageSampler = Graphics::Vulkan::Sampler::Create(device);
    Graphics::Vulkan::DescriptorSet::UpdateDescriptorSets(AlbedoDescriptorSet, {AlbedoImageView}, {AlbedoImageSampler});
  }
  ~Mesh()
  {
    for (auto device : Core::Engine::GetAllDevices())
      Graphics::Vulkan::Device::WaitForDevice(device);

    Graphics::Vulkan::CommandPool::Destroy(RenderCommandPool);
    Graphics::Vulkan::CommandPool::Destroy(TransferCommandPool);
    Graphics::Vulkan::CommandPool::Destroy(GraphicsProcessCommandPool);

    //mesh information
    Graphics::Vulkan::Buffer::Destroy(StagingVertexBuffer);
    Graphics::Vulkan::Buffer::Destroy(VertexBuffer);
    Graphics::Vulkan::Buffer::Destroy(StagingIndexBuffer);
    Graphics::Vulkan::Buffer::Destroy(IndexBuffer);

    //transform buffer
    Graphics::Vulkan::DescriptorPool::Destroy(TransformDescriptorPool);
    Graphics::Vulkan::Buffer::Destroy(StagingTransformBuffer);
    Graphics::Vulkan::Buffer::Destroy(TransformBuffer);

    //light information
    Graphics::Vulkan::DescriptorPool::Destroy(LightDescriptorPool);
    Graphics::Vulkan::Buffer::Destroy(StagingLightBuffer);
    Graphics::Vulkan::Buffer::Destroy(LightBuffer);

    //material
    Graphics::Vulkan::DescriptorPool::Destroy(MaterialDescriptorPool);
    Graphics::Vulkan::Sampler::Destroy(AlbedoImageSampler);
    Graphics::Vulkan::ImageView::Destroy(AlbedoImageView);
    Graphics::Vulkan::Image::Destroy(AlbedoImage);
    Graphics::Vulkan::Buffer::Destroy(StagingAlbedoImage);
  }

  void UpdateBuffers(Transform *transform)
  {
    if (IsDirty)
    {
      if (StagingVertexBuffer.Size != VertexBuffer.Size)
      {
        Graphics::Vulkan::Buffer::Destroy(VertexBuffer);
        VertexBuffer = Graphics::Vulkan::Buffer::Create(device, StagingVertexBuffer.Size, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
        Graphics::Vulkan::Command::CopyBuffer(TransferCommand, StagingVertexBuffer, VertexBuffer);
      }
      if (StagingIndexBuffer.Size != IndexBuffer.Size)
      {
        Graphics::Vulkan::Buffer::Destroy(IndexBuffer);
        IndexBuffer = Graphics::Vulkan::Buffer::Create(device, StagingIndexBuffer.Size, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
        Graphics::Vulkan::Command::CopyBuffer(TransferCommand, StagingIndexBuffer, IndexBuffer);
      }
      Graphics::Vulkan::Command::CopyBuffer(TransferCommand, StagingLightBuffer, LightBuffer);
    }
    if (transform != nullptr && transform->GetIsDirty())
    {
      auto model = glm::mat4(1.0);
      model = transform->GetModelMatrix();
      Graphics::Vulkan::Buffer::SetData(StagingTransformBuffer, &model, sizeof(glm::mat4));
      Graphics::Vulkan::Command::CopyBuffer(TransferCommand, StagingTransformBuffer, TransformBuffer);
    }
  }

  void UpdateMaterial(Material *material)
  {
    if (material->Albedo.ByteSize != StagingAlbedoImage.Size)
    {
      //destroy old buffers
      Graphics::Vulkan::Buffer::Destroy(StagingAlbedoImage);
      Graphics::Vulkan::Image::Destroy(AlbedoImage);
      Graphics::Vulkan::ImageView::Destroy(AlbedoImageView);

      //create new buffers
      StagingAlbedoImage = Graphics::Vulkan::Buffer::CreateHostSrc(device, material->Albedo.ByteSize);
      AlbedoImage = Graphics::Vulkan::Image::Create(device, material->Albedo.Width, material->Albedo.Height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
      AlbedoImageView = Graphics::Vulkan::ImageView::Create(device, AlbedoImage, VK_IMAGE_ASPECT_COLOR_BIT);
      Graphics::Vulkan::DescriptorSet::UpdateDescriptorSets(AlbedoDescriptorSet, {AlbedoImageView}, {AlbedoImageSampler});

      //transfer data
      Graphics::Vulkan::Buffer::SetData(StagingAlbedoImage, material->Albedo.Pixels.data(), material->Albedo.ByteSize);
      Graphics::Vulkan::Command::TransferImageLayout(GraphicsProcessCommand, AlbedoImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      Graphics::Vulkan::Command::BufferToImage(GraphicsProcessCommand, StagingAlbedoImage, AlbedoImage, {0, 0}, {AlbedoImage.Width, AlbedoImage.Height});
      Graphics::Vulkan::Command::TransferImageLayout(GraphicsProcessCommand, AlbedoImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
  }

  void SetVertices(std::vector<Graphics::Vertex> vertices)
  {
    Vertices = vertices;

    Graphics::Vulkan::Buffer::Destroy(StagingVertexBuffer);
    uint32_t verticesSize = sizeof(Graphics::Vertex) * vertices.size();
    StagingVertexBuffer = Graphics::Vulkan::Buffer::CreateHostSrc(device, verticesSize);
    if (vertices.size() > 0)
      Graphics::Vulkan::Buffer::SetData(StagingVertexBuffer, vertices.data(), verticesSize);
    IsDirty = true;
  }
  void SetIndices(std::vector<uint32_t> indices)
  {
    Indices = indices;

    Graphics::Vulkan::Buffer::Destroy(StagingIndexBuffer);
    uint32_t verticesSize = sizeof(Graphics::Vertex) * indices.size();
    StagingIndexBuffer = Graphics::Vulkan::Buffer::CreateHostSrc(device, verticesSize);
    if (indices.size() > 0)
      Graphics::Vulkan::Buffer::SetData(StagingIndexBuffer, indices.data(), sizeof(uint32_t) * indices.size());
    IsDirty = true;
  }
  void SetLights(std::vector<Core::Entity::Entity *> lights)
  {
    Lights = lights;

    const auto lightInfos = GetLightInfos();
    const auto lightInfoSize = lightInfos.size();
    Graphics::Vulkan::Buffer::SetData(StagingLightBuffer, &lightInfoSize, StagingLightBuffer.Size);
    if (lightInfos.size() > 0)
      Graphics::Vulkan::Buffer::SetData(StagingLightBuffer, lightInfos.data(), sizeof(lightInfos[0]) * lightInfos.size(), sizeof(glm::vec4));
    IsDirty = true;
  }

  std::vector<Graphics::Vertex> GetVertices()
  {
    return Vertices;
  }
  std::vector<uint32_t> GetIndices()
  {
    return Indices;
  }
};
} // namespace Component
} // namespace Tortuga

#endif