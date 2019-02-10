#ifndef VULKAN_API_UNIFORM_BUFFER_OBJECT
#define VULKAN_API_UNIFORM_BUFFER_OBJECT

#include "../../Core.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif