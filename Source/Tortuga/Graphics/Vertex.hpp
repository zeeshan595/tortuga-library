#ifndef _VERTEX
#define _VERTEX

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <vector>

namespace Tortuga
{
namespace Graphics
{
struct Vertex
{
  glm::vec3 Position;
  glm::vec2 Texture;
  glm::vec3 Normal;

  static std::vector<VkVertexInputBindingDescription> GetBindingDescription()
  {
    std::vector<VkVertexInputBindingDescription> bindingDescription(1);

    //positions
    bindingDescription[0].binding = 0;
    bindingDescription[0].stride = sizeof(Vertex);
    bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
  {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

    //position
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, Vertex::Position);

    //textures
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, Vertex::Texture);

    //normals
    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, Vertex::Normal);

    return attributeDescriptions;
  }
};
} // namespace Graphics
} // namespace Tortuga

#endif