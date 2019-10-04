#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include <cstdint>
#include <future>

#include "../Core/ECS/System.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/DisplaySurface.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/Shader.hpp"
#include "../Graphics/Vulkan/Pipeline.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Core/Engine.hpp"
#include "../Graphics/AcceleratedMesh.hpp"
#include "../Components/Mesh.hpp"

namespace Tortuga
{
namespace Systems
{
struct Rendering : public Core::ECS::System
{
private:
  struct RenderOptions
  {
    uint32_t renderWidth;
    uint32_t renderHeight;
  };
  Graphics::Vulkan::Instance::Instance VulkanInstance;
  Graphics::DisplaySurface::DisplaySurface DisplaySurface;
  Graphics::Vulkan::CommandPool::CommandPool GraphicsCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool ComputeCommandPool;
  Graphics::Vulkan::CommandPool::CommandPool TransferCommandPool;

  //mesh
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout MeshDescriptorLayout;
  Graphics::Vulkan::DescriptorPool::DescriptorPool MeshDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet MeshDescriptorSet;
  Graphics::Vulkan::Buffer::Buffer MeshBufferCounts;
  Graphics::Vulkan::Buffer::Buffer MeshBufferData;
  Graphics::Vulkan::Buffer::Buffer MeshBufferIndices;
  Graphics::Vulkan::Buffer::Buffer MeshBufferFaces;
  Graphics::Vulkan::Buffer::Buffer MeshBufferNodes;
  Graphics::Vulkan::Command::Command MeshCopyCommand;

  //renderer
  Graphics::Vulkan::Shader::Shader RenderShader;
  Graphics::Vulkan::Pipeline::Pipeline RenderPipeline;
  Graphics::Vulkan::Buffer::Buffer RenderOptionsBuffer;
  Graphics::Vulkan::Buffer::Buffer RenderingBuffer;
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout RenderingDescriptorLayout;
  Graphics::Vulkan::DescriptorPool::DescriptorPool RenderDescriptorPool;
  Graphics::Vulkan::DescriptorSet::DescriptorSet RenderDescriptorSet;
  Graphics::Vulkan::Image::Image RenderedImage;
  Graphics::Vulkan::Command::Command RenderCommand;
  Graphics::Vulkan::Command::Command PresentCommand;

  //sync
  Graphics::Vulkan::Semaphore::Semaphore MeshSemaphore;
  Graphics::Vulkan::Semaphore::Semaphore RenderSemaphore;
  Graphics::Vulkan::Semaphore::Semaphore PresentSemaphore;
  Graphics::Vulkan::Fence::Fence PresentFence;

  void SetupLayoutInformation(Graphics::Vulkan::Device::Device device);
  void ProcessMeshBuffers(Graphics::Vulkan::Device::Device device);

public:
  struct MeshView : public Core::ECS::Component
  {
    Graphics::Vulkan::Device::Device DeviceInUse;
    Graphics::Vulkan::Buffer::Buffer StagingMeshCounts;
    Graphics::Vulkan::Buffer::Buffer StagingMeshData;
    Graphics::Vulkan::Buffer::Buffer StagingIndices;
    Graphics::Vulkan::Buffer::Buffer StagingFaces;
    Graphics::Vulkan::Buffer::Buffer StagingNodes;

    void OnCreate() override
    {
      const auto device = DeviceInUse;
      const auto meshComp = Core::Engine::GetComponent<Components::Mesh>(Root);
      const auto mesh = meshComp->GetMesh();
      StagingMeshData = Graphics::Vulkan::Buffer::CreateHost(device, mesh.MeshDataByteSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      uint32_t offset = 0;
      Graphics::Vulkan::Buffer::SetData(StagingMeshData, mesh.Positions.data(), sizeof(glm::vec3) * mesh.Positions.size(), offset);
      offset += sizeof(glm::vec3) * mesh.Positions.size();
      Graphics::Vulkan::Buffer::SetData(StagingMeshData, mesh.Textures.data(), sizeof(glm::vec2) * mesh.Textures.size(), offset);
      offset += sizeof(glm::vec2) * mesh.Textures.size();
      Graphics::Vulkan::Buffer::SetData(StagingMeshData, mesh.Normals.data(), sizeof(glm::vec3) * mesh.Normals.size(), offset);
      StagingIndices = Graphics::Vulkan::Buffer::CreateHost(device, sizeof(Graphics::AcceleratedMesh::IndexStruct) * mesh.Indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      Graphics::Vulkan::Buffer::SetData(StagingIndices, mesh.Indices.data(), sizeof(Graphics::AcceleratedMesh::IndexStruct) * mesh.Indices.size());
      StagingFaces = Graphics::Vulkan::Buffer::CreateHost(device, sizeof(Graphics::AcceleratedMesh::Face) * mesh.Faces.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      Graphics::Vulkan::Buffer::SetData(StagingFaces, mesh.Faces.data(), sizeof(Graphics::AcceleratedMesh::Face) * mesh.Faces.size());
      StagingNodes = Graphics::Vulkan::Buffer::CreateHost(device, sizeof(Graphics::AcceleratedMesh::Node) * mesh.Nodes.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      Graphics::Vulkan::Buffer::SetData(StagingNodes, mesh.Nodes.data(), sizeof(Graphics::AcceleratedMesh::Node) * mesh.Nodes.size());

      std::vector<uint32_t> meshCounts;
      meshCounts.push_back(mesh.Positions.size());
      meshCounts.push_back(mesh.Textures.size());
      meshCounts.push_back(mesh.Normals.size());
      meshCounts.push_back(mesh.Faces.size());
      meshCounts.push_back(mesh.Nodes.size());
      StagingMeshCounts = Graphics::Vulkan::Buffer::CreateHost(device, sizeof(uint32_t) * meshCounts.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
      Graphics::Vulkan::Buffer::SetData(StagingMeshCounts, meshCounts.data(), sizeof(uint32_t) * meshCounts.size());
      meshComp->IsDirty = false;
    }

    void OnDestroy() override
    {
      Graphics::Vulkan::Buffer::Destroy(StagingMeshCounts);
      Graphics::Vulkan::Buffer::Destroy(StagingMeshData);
      Graphics::Vulkan::Buffer::Destroy(StagingIndices);
      Graphics::Vulkan::Buffer::Destroy(StagingFaces);
      Graphics::Vulkan::Buffer::Destroy(StagingNodes);
    }
  };

  void Update() override;
  Rendering();
  ~Rendering();
};
} // namespace Systems
} // namespace Tortuga

#endif