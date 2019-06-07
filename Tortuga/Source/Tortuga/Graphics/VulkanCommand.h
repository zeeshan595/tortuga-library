#ifndef _VULKAN_COMMAND
#define _VULKAN_COMMAND

#include "../Console.h"
#include "./VulkanDevice.h"
#include "./VulkanErrorHandler.h"
#include "./VulkanImage.h"
#include "./VulkanPipeline.h"

#include <glm/glm.hpp>
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
void VulkanCommandBegin(VulkanCommand command, VkCommandBufferUsageFlags usageFlag);
void VulkanCommandEnd(VulkanCommand command);
void VulkanCommandSubmit(std::vector<VulkanCommand> command,
                         VulkanQueueType queueType);
void VulkanCommandBindPipeline(VulkanCommand command, VulkanPipeline pipeline);
void VulkanCommandDispatch(VulkanCommand command, uint32_t groupCountX,
                           uint32_t groupCountY, uint32_t groupCountZ);
void VulkanCommandCopyBufferToImage(VulkanCommand command, VulkanBuffer buffer,
                                    VkImage image, glm::vec2 imageOffset,
                                    glm::vec2 imageSize);
void VulkanCommandImageLayoutTransfer(VulkanCommand command, VkImage image,
                                      VkImageLayout oldLayout,
                                      VkImageLayout newLayout);
void VulkanCommandCopyImage(VulkanCommand command, VkImage source,
                            VkImageLayout sourceLayout, VkImage destination,
                            VkImageLayout destinationLayout, glm::vec2 offset,
                            glm::vec2 size);
void VulkanCommandBlitImage(VulkanCommand command, VkImage source,
                            VkImageLayout sourceLayout, VkImage destination,
                            VkImageLayout destinationLayout, glm::vec2 offset,
                            glm::vec2 size);
} // namespace Graphics
} // namespace Tortuga

#endif