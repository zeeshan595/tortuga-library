#ifndef _VERTEX
#define _VERTEX

#include "../Core.h"

namespace Tortuga
{
namespace Graphics
{
struct Vertex
{
  glm::vec3 position;
  glm::vec3 color;

  static VkVertexInputBindingDescription GetBindingDescription()
  {
    auto bindingDescription = VkVertexInputBindingDescription();
    {
      bindingDescription.binding = 0;
      bindingDescription.stride = sizeof(Vertex);
      bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    return bindingDescription;
  }
  static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
  {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    {
      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(Vertex, position);

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset = offsetof(Vertex, color);
    }
    return attributeDescriptions;
  }
};
}; // namespace Graphics
}; // namespace Tortuga

#endif