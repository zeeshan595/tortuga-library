#include "./Mesh.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Screen
{
Vulkan::Device::Device GetDevice();
Vulkan::DescriptorLayout::DescriptorLayout GetMeshPreProcessorDescriptorLayout();
} // namespace Screen

Mesh::Mesh()
{
  auto device = Screen::GetDevice();
  this->Staging = Vulkan::Buffer::CreateHostSrc(device, MESH_SIZE);
  this->Buffer = Vulkan::Buffer::CreateDeviceOnlyDest(device, MESH_SIZE);
  this->DescriptorPool = Vulkan::DescriptorPool::Create(device, Screen::GetMeshPreProcessorDescriptorLayout());
}
Mesh::~Mesh()
{
  Vulkan::DescriptorPool::Destroy(this->DescriptorPool);
  Vulkan::Buffer::Destroy(this->Staging);
  Vulkan::Buffer::Destroy(this->Buffer);
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
  this->BufferData.IsDirty = true;
}

void Mesh::SetVertices(std::vector<Vertex> vertices)
{
  if (vertices.size() > MAX_VERTICES_SIZE)
  {
    Console::Warning("Vertices length is too large!");
    return;
  }
  memcpy(this->BufferData.Vertices, vertices.data(), vertices.size() * sizeof(Vertex));
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
} // namespace Graphics
} // namespace Tortuga