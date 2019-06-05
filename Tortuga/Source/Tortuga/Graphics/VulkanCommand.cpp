#include "./VulkanCommand.h"

namespace Tortuga {
namespace Graphics {
VulkanCommandPool CreateVulkanCommandPool(VulkanDevice device) {
  auto data = VulkanCommandPool();
  data.Device = device;

  auto commandPoolInfo = VkCommandPoolCreateInfo();
  {
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.pNext = 0;
    commandPoolInfo.flags = 0;
    commandPoolInfo.queueFamilyIndex =
        device.QueueFamilies.ComputeFamily.value();
  }
  Graphics::ErrorCheck(vkCreateCommandPool(
      device.VirtualDevice, &commandPoolInfo, nullptr, &data.CommandPool));

  return data;
}
void DestroyVulkanCommandPool(VulkanCommandPool data) {
  vkDestroyCommandPool(data.Device.VirtualDevice, data.CommandPool, nullptr);
}
VulkanCommand CreateVulkanCommand(VulkanCommandPool commandPool) {
  auto data = VulkanCommand();
  data.CommandPool = commandPool;

  auto commandBufferAllocateInfo = VkCommandBufferAllocateInfo();
  {
    commandBufferAllocateInfo.sType =
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = 0;
    commandBufferAllocateInfo.commandPool = data.CommandPool.CommandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 1;
  }
  Graphics::ErrorCheck(vkAllocateCommandBuffers(
      data.CommandPool.Device.VirtualDevice, &commandBufferAllocateInfo,
      &data.CommandBuffer));

  return data;
}
void VulkanCommandBegin(VulkanCommand command) {
  auto commandBufferBeginInfo = VkCommandBufferBeginInfo();
  {
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = 0;
    commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    commandBufferBeginInfo.pInheritanceInfo = 0;
  }
  ErrorCheck(
      vkBeginCommandBuffer(command.CommandBuffer, &commandBufferBeginInfo));
}
void VulkanCommandEnd(VulkanCommand command) {
  ErrorCheck(vkEndCommandBuffer(command.CommandBuffer));
}
void VulkanCommandBindPipeline(VulkanCommand command, VulkanPipeline pipeline) {
  vkCmdBindPipeline(command.CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                    pipeline.Pipeline);
  vkCmdBindDescriptorSets(command.CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                          pipeline.PipelineLayout, 0, 1,
                          &pipeline.DescriptorSets, 0, 0);
}
void VulkanCommandDispatch(VulkanCommand command, uint32_t groupCountX,
                           uint32_t groupCountY, uint32_t groupCountZ) {
  vkCmdDispatch(command.CommandBuffer, groupCountX, groupCountY, groupCountZ);
}
void VulkanCommandSubmit(std::vector<VulkanCommand> command, VkQueue queue) {
  if (command.size() == 0) {
    Console::Error("You must provide atleast 1 command to submit");
    return;
  }
  std::vector<VkCommandBuffer> cmdBuffers(command.size());
  for (uint32_t i = 0; i < command.size(); i++) {
    cmdBuffers[i] = command[i].CommandBuffer;
  }

  auto submitInfo = VkSubmitInfo();
  {
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = 0;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = 0;
    submitInfo.pWaitDstStageMask = 0;
    submitInfo.commandBufferCount = cmdBuffers.size();
    submitInfo.pCommandBuffers = cmdBuffers.data();
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;
  }
  ErrorCheck(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
}
} // namespace Graphics
} // namespace Tortuga