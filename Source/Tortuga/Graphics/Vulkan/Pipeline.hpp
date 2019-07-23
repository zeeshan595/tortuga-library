#ifndef _VULKAN_PIPELINE
#define _VULKAN_PIPELINE

#include <vulkan/vulkan.h>

#include "./ErrorCheck.hpp"
#include "./Device.hpp"
#include "./DescriptorLayout.hpp"
#include "./Shader.hpp"

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

Pipeline CreateComputePipeline(
    Device::Device device,
    Shader::Shader shader,
    std::vector<char> cache = {},
    std::vector<DescriptorLayout::DescriptorLayout> layouts = {});
void DestroyPipeline(Pipeline data);
std::vector<char> GetCacheData(Pipeline data);
} // namespace Pipeline
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif