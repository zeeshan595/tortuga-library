#ifndef _VULKAN_COMMAND
#define _VULKAN_COMMAND

#include "./VulkanDevice.h"
#include "./VulkanErrorHandler.h"
#include "./VulkanPipeline.h"
#include "../Console.h"

#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
struct VulkanCommandPool {
  VulkanDevice Device;
  VkCommandPool CommandPool;
};
struct VulkanCommand {
  VulkanCommandPool CommandPool;
  VkCommandBuffer CommandBuffer;
};

VulkanCommandPool CreateVulkanCommandPool(VulkanDevice device);
void DestroyVulkanCommandPool(VulkanCommandPool data);

VulkanCommand CreateVulkanCommand(VulkanCommandPool commandPool);
void VulkanCommandBegin(VulkanCommand command);
void VulkanCommandEnd(VulkanCommand command);
void VulkanCommandSubmit(std::vector<VulkanCommand> command, VkQueue queue);
void VulkanCommandBindPipeline(VulkanCommand command, VulkanPipeline pipeline);
void VulkanCommandDispatch(VulkanCommand command, uint32_t groupCountX,
                           uint32_t groupCountY, uint32_t groupCountZ);
} // namespace Graphics
} // namespace Tortuga

#endif