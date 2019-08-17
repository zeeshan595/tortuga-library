#include "./Mesh.hpp"

namespace Tortuga
{
namespace Component
{
class MeshDescriptorLayout
{
public:
  Graphics::Vulkan::DescriptorLayout::DescriptorLayout meshDescriptorLayout;

  MeshDescriptorLayout()
  {
    meshDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(Core::Engine::GetMainDevice(), 1);
  }
  ~MeshDescriptorLayout()
  {
    Graphics::Vulkan::DescriptorLayout::Destroy(meshDescriptorLayout);
  }
};
MeshDescriptorLayout DescriptorLayout = MeshDescriptorLayout();
Graphics::Vulkan::DescriptorLayout::DescriptorLayout GetMeshDescriptorLayout()
{
  return DescriptorLayout.meshDescriptorLayout;
}
void Mesh::ResetTransformation()
{
  this->BufferData.Transformation = glm::mat4x4(1.0f);
}

void Mesh::ApplyTransformation(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale)
{
  this->BufferData.Transformation = glm::mat4x4(1.0f);
  this->BufferData.Transformation = glm::scale(this->BufferData.Transformation, scale);
  this->BufferData.Transformation = glm::rotate(this->BufferData.Transformation, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
  this->BufferData.Transformation = glm::translate(this->BufferData.Transformation, position);
}

void Mesh::SetVertices(std::vector<Graphics::Vertex> vertices)
{
  if (vertices.size() > MAX_VERTICES_SIZE)
  {
    Console::Warning("Vertices length is too large!");
    return;
  }
  memcpy(this->BufferData.Vertices, vertices.data(), vertices.size() * sizeof(Graphics::Vertex));
  this->BufferData.VerticesSize = vertices.size();
}

void Mesh::SetIndices(std::vector<uint32_t> indices)
{
  if (indices.size() > MAX_INDICES_SIZE)
  {
    Console::Warning("Indices length is too large!");
    return;
  }
  if (indices.size() % 3 != 0)
  {
    Console::Warning("Indices' length must be a multiple of 3!");
    return;
  }
  memcpy(this->BufferData.Indices, indices.data(), indices.size() * sizeof(uint32_t));
  this->BufferData.IndicesSize = indices.size();
}

Mesh::Mesh()
{
  this->DescriptorPool = Graphics::Vulkan::DescriptorPool::Create(Core::Engine::GetMainDevice(), {DescriptorLayout.meshDescriptorLayout});
  this->DescriptorSets = Graphics::Vulkan::DescriptorSets::Create(Core::Engine::GetMainDevice(), this->DescriptorPool, DescriptorLayout.meshDescriptorLayout);
  this->Staging = Graphics::Vulkan::Buffer::CreateHostSrc(Core::Engine::GetMainDevice(), MESH_SIZE);
  this->Buffer = Graphics::Vulkan::Buffer::CreateDeviceOnly(Core::Engine::GetMainDevice(), MESH_SIZE);
  Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(this->DescriptorSets, {this->Buffer});
  this->CommandPool = Graphics::Vulkan::CommandPool::Create(Core::Engine::GetMainDevice(), Core::Engine::GetMainDevice().QueueFamilies.Compute.Index);
  this->Command = Graphics::Vulkan::Command::Create(Core::Engine::GetMainDevice(), this->CommandPool, Graphics::Vulkan::Command::PRIMARY);
}
Mesh::~Mesh()
{
  Graphics::Vulkan::Buffer::Destroy(this->Staging);
  Graphics::Vulkan::Buffer::Destroy(this->Buffer);
  Graphics::Vulkan::DescriptorPool::Destroy(this->DescriptorPool);
  Graphics::Vulkan::CommandPool::Destroy(this->CommandPool);
}
} // namespace Component
} // namespace Tortuga