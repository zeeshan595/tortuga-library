#ifndef _MESH
#define _MESH

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "../Graphics/Vulkan/Device.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vertex.hpp"

namespace Tortuga
{
namespace Component
{
struct Mesh
{
private:

public:
  Graphics::Vulkan::Command::Command RenderCommand;
  Graphics::Vulkan::Buffer::Buffer VertexBuffer;
  std::vector<Graphics::Vertex> Vertices;

  Mesh()
  {
    RenderCommand.Command = VK_NULL_HANDLE;
    VertexBuffer.Buffer = VK_NULL_HANDLE;
  }
};
} // namespace Component
} // namespace Tortuga

#endif