#include "./Mesh.hpp"

namespace Tortuga
{
namespace Component
{
std::vector<Graphics::Vulkan::DescriptorLayout::Binding> bindings(2);
{
  //input
  bindings[0].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  bindings[0].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
  bindings[0].DescriptorCount = 1;
  bindings[0].Sampler = VK_NULL_HANDLE;

  //output
  bindings[1].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  bindings[1].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
  bindings[1].DescriptorCount = 1;
  bindings[1].Sampler = VK_NULL_HANDLE;
}
MeshDescriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(Core::Engine::GetMainDevice(), bindings);

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
  this->DescriptorPool = Graphics::Vulkan::DescriptorPool::Create(Core::Engine::GetMainDevice(), MeshDescriptorLayout);
  this->DescriptorSets = Graphics::Vulkan::DescriptorSets::Create(Core::Engine::GetMainDevice(), this->DescriptorPool, {MeshDescriptorLayout});

  this->Staging = Graphics::Vulkan::Buffer::CreateHostSrc(Core::Engine::GetMainDevice(), MESH_SIZE);
  this->Buffer = Graphics::Vulkan::Buffer::CreateDeviceOnlyDest(Core::Engine::GetMainDevice(), MESH_SIZE);
}
Mesh::~Mesh()
{
  Graphics::Vulkan::Buffer::Destroy(this->Staging);
  Graphics::Vulkan::Buffer::Destroy(this->Buffer);

  Graphics::Vulkan::DescriptorSets::Destroy(this->DescriptorSets);
  Graphics::Vulkan::DescriptorPool::Destroy(this->DescriptorPool);
  Graphics::Vulkan::DescriptorLayout::Destroy(this->DescriptorLayout);
}
} // namespace Component
} // namespace Tortuga