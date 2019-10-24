#ifndef _VULKAN_PIPELINE
#define _VULKAN_PIPELINE

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./DescriptorLayout.hpp"
#include "./Shader.hpp"
#include "./RenderPass.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Pipeline
{
struct Pipeline
{
  VkDevice Device = VK_NULL_HANDLE;
  VkPipelineLayout Layout = VK_NULL_HANDLE;
  VkPipeline Pipeline = VK_NULL_HANDLE;
  VkRenderPass RenderPass = VK_NULL_HANDLE;
  VkPipelineCache Cache = VK_NULL_HANDLE;
};

Pipeline CreateGraphicsPipeline(
    Device::Device device,
    std::vector<Shader::Shader> shaders,
    RenderPass::RenderPass renderPass,
    std::vector<VkVertexInputBindingDescription> bindings = {},
    std::vector<VkVertexInputAttributeDescription> attributes = {},
    std::vector<DescriptorLayout::DescriptorLayout> descriptorSetLayouts = {});
Pipeline CreateComputePipeline(
    Device::Device device,
    Shader::Shader shader,
    std::vector<char> cache,
    std::vector<DescriptorLayout::DescriptorLayout> layouts);
void Destroy(Pipeline data);
std::vector<char> GetCacheData(Pipeline data);
} // namespace Pipeline
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif