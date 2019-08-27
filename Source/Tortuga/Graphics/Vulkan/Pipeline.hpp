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
  VkDevice Device;
  VkPipelineLayout Layout;
  VkPipeline Pipeline;
  VkRenderPass RenderPass;
  VkPipelineCache Cache;
};

Pipeline CreateGraphicsPipeline(
    Device::Device device,
    Shader::Shader vertexShader,
    Shader::Shader fragmentShader,
    RenderPass::RenderPass renderPass,
    uint32_t width, uint32_t height,
    std::vector<VkVertexInputBindingDescription> bindings = {},
    std::vector<VkVertexInputAttributeDescription> attributes = {});
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