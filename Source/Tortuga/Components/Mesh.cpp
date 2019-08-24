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
  this->BufferData.Transformation = glm::mat4(1.0f);
  this->BufferData.NormalMatrix = glm::mat4(1.0);
}

void Mesh::ApplyTransformation(glm::vec3 position, glm::vec4 rotation, glm::vec3 scale)
{
  this->BufferData.Transformation = glm::mat4(1.0f);
  this->BufferData.Transformation = glm::translate(this->BufferData.Transformation, position);
  this->BufferData.Transformation = glm::rotate(this->BufferData.Transformation, rotation.y, glm::vec3(0, rotation.w, 0));
  this->BufferData.Transformation = glm::rotate(this->BufferData.Transformation, rotation.x, glm::vec3(rotation.w, 0, 0));
  this->BufferData.Transformation = glm::rotate(this->BufferData.Transformation, rotation.z, glm::vec3(0, 0, rotation.w));
  this->BufferData.Transformation = glm::scale(this->BufferData.Transformation, scale);

  this->BufferData.NormalMatrix = glm::mat4(1.0);
  this->BufferData.NormalMatrix = glm::rotate(this->BufferData.NormalMatrix, rotation.y, glm::vec3(0, rotation.w, 0));
  this->BufferData.NormalMatrix = glm::rotate(this->BufferData.NormalMatrix, rotation.x, glm::vec3(rotation.w, 0, 0));
  this->BufferData.NormalMatrix = glm::rotate(this->BufferData.NormalMatrix, rotation.z, glm::vec3(0, 0, rotation.w));
}

void Mesh::SetVertices(std::vector<glm::vec4> vertices, bool recalculateBounds)
{
  if (vertices.size() > MAX_VERTICES_SIZE)
  {
    Console::Warning("Vertices length is too large!");
    return;
  }
  memcpy(this->BufferData.Vertices, vertices.data(), vertices.size() * sizeof(glm::vec4));
  this->BufferData.VerticesSize = vertices.size();
  this->BufferData.Center = glm::vec4(0, 0, 0, 1);

  if (recalculateBounds)
  {
    //calculate bounds
    this->BufferData.Bounds = glm::length(glm::abs(this->BufferData.Vertices[0]));
    for (uint32_t i = 0; i < this->BufferData.VerticesSize; i++)
    {
      float len = glm::length(glm::abs(this->BufferData.Vertices[i]));
      if (len > this->BufferData.Bounds)
        this->BufferData.Bounds = len;
    }
  }
}

void Mesh::SetNormals(std::vector<glm::vec4> normals)
{
  if (normals.size() > MAX_VERTICES_SIZE)
  {
    Console::Warning("Normals length is too large!");
    return;
  }
  memcpy(this->BufferData.Normals, normals.data(), normals.size() * sizeof(glm::vec4));
  this->BufferData.NormalsSize = normals.size();
}

void Mesh::SetTextures(std::vector<glm::vec4> textures)
{
  if (textures.size() > MAX_VERTICES_SIZE)
  {
    Console::Warning("Normals length is too large!");
    return;
  }
  memcpy(this->BufferData.Textures, textures.data(), textures.size() * sizeof(glm::vec4));
  this->BufferData.TexturesSize = textures.size();
}

void Mesh::SetIndices(std::vector<Graphics::Index> indices)
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
  memcpy(this->BufferData.Indices, indices.data(), indices.size() * sizeof(Graphics::Index));
  this->BufferData.IndicesSize = indices.size();
}

void Mesh::SetStatic()
{
  this->IsStatic = true;
}
void Mesh::SetDynamic()
{
  this->IsStatic = false;
}

Mesh::Mesh()
{
  this->IsStatic = false;
  this->IsProcessedOnce = false;
  this->BufferData.Transformation = glm::mat4(1.0);
  this->BufferData.NormalMatrix = glm::mat4(1.0);
  this->DescriptorPool = Graphics::Vulkan::DescriptorPool::Create(Core::Engine::GetMainDevice(), {DescriptorLayout.meshDescriptorLayout});
  this->DescriptorSets = Graphics::Vulkan::DescriptorSet::Create(Core::Engine::GetMainDevice(), this->DescriptorPool, DescriptorLayout.meshDescriptorLayout);
  this->Staging = Graphics::Vulkan::Buffer::CreateHost(Core::Engine::GetMainDevice(), MESH_SIZE);
  this->Buffer = Graphics::Vulkan::Buffer::CreateDeviceOnly(Core::Engine::GetMainDevice(), MESH_SIZE);
  Graphics::Vulkan::DescriptorSet::UpdateDescriptorSets(this->DescriptorSets, {this->Buffer});
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