#ifndef _VULKAN_COMMAND
#define _VULKAN_COMMAND

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./Pipeline.hpp"
#include "./CommandPool.hpp"
#include "./DescriptorSets.hpp"
#include "./Buffer.hpp"
#include "./Image.hpp"
#include "./Semaphore.hpp"
#include "./Fence.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Command
{
enum Type
{
  PRIMARY,
  SECONDARY
};
struct Command
{
  VkDevice Device;
  VkCommandBuffer Command;
};

Command Create(Device::Device device, CommandPool::CommandPool pool, Type type);
std::vector<Command> Create(Device::Device device, CommandPool::CommandPool pool, Type type, uint32_t amount);

void Begin(Command data, VkCommandBufferUsageFlags usage);
void End(Command data);
void CopyBuffer(Command data, Buffer::Buffer source, Buffer::Buffer destination, uint32_t sourceOffset = 0, uint32_t destinationOffset = 0);
void BindPipeline(Command data, VkPipelineBindPoint BindPoint, Pipeline::Pipeline pipeline, std::vector<DescriptorSets::DescriptorSets> descriptorSets = {});
void Compute(Command data, uint32_t x, uint32_t y, uint32_t z);
void Submit(std::vector<Command> data, VkQueue queue, std::vector<Semaphore::Semaphore> wait = {}, std::vector<Semaphore::Semaphore> signal = {}, Fence::Fence fence = {VK_NULL_HANDLE, VK_NULL_HANDLE});
void TransferImageLayout(Command data, Image::Image image, VkImageLayout oldLayout, VkImageLayout newLayout);
void BufferToImage(Command data, Buffer::Buffer buffer, Image::Image image, glm::vec2 offset, glm::vec2 size);
void CopyImage(Command data, Image::Image source, Image::Image destination);
void BlitImage(Command data, Image::Image source, Image::Image destination, glm::vec2 size, glm::vec2 destinationOffset, glm::vec2 sourceOffset);

} // namespace Command
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif