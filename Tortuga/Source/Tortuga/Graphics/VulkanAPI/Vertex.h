#ifndef _VERTEX
#define _VERTEX

#include "../../Core.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture;

    static VkVertexInputBindingDescription GetBindingPerDescription(uint32_t bindingIdentity, bool perInstance = false)
    {
        auto bindingDescription = VkVertexInputBindingDescription();
        {
            bindingDescription.binding = bindingIdentity;
            bindingDescription.stride = sizeof(Vertex);
            if (perInstance == false)
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            else
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
        }
        return bindingDescription;
    }

    static std::vector<VkVertexInputAttributeDescription> GetVertexDescription(uint32_t bindingIdentity)
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescription(3);
        {
            //position
            attributeDescription[0].binding = bindingIdentity;
            attributeDescription[0].location = 0;
            attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription[0].offset = offsetof(Vertex, position);

            //color
            attributeDescription[1].binding = bindingIdentity;
            attributeDescription[1].location = 1;
            attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescription[1].offset = offsetof(Vertex, color);

            //texture
            attributeDescription[2].binding = bindingIdentity;
            attributeDescription[2].location = 2;
            attributeDescription[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescription[2].offset = offsetof(Vertex, texture);
        }
        return attributeDescription;
    }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif