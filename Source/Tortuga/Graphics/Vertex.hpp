#ifndef _GRAPHICS_INDEX
#define _GRAPHICS_INDEX

#include <vulkan/vulkan.h>

#include <fstream>
#include <vector>
#include <glm/glm.hpp>

namespace Tortuga
{
namespace Graphics
{
struct Vertex
{
  glm::vec2 Position;
  glm::vec3 Color;

  static VkVertexInputBindingDescription getBindingDescription()
  {
    VkVertexInputBindingDescription bindingDescription = {};
    {
      bindingDescription.binding = 0;
      bindingDescription.stride = sizeof(Vertex);
      bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }
    return bindingDescription;
  }

  static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
  {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    {
      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(Vertex, Position);

      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset = offsetof(Vertex, Color);
    }
    return attributeDescriptions;
  }
};
} // namespace Graphics
} // namespace Tortuga

#endif