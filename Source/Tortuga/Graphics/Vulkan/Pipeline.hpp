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
struct Viewport
{
  float x = 0.0f;
  float y = 0.0f;
  float width = 800.0f;
  float height = 600.0f;
  float minDepth = 0.01f;
  float maxDepth = 1000.0f;
};
struct Pipeline
{
  VkDevice Device;
  VkPipelineLayout Layout;
  VkPipeline Pipeline;
  VkPipelineCache Cache;
};

Pipeline CreateComputePipeline(Device::Device device, std::vector<DescriptorLayout::DescriptorLayout> layouts, Shader::Shader shader);
Pipeline CreateGraphicsPipeline(Device::Device device, std::vector<DescriptorLayout::DescriptorLayout> layouts, Shader::Shader vertex, Shader::Shader fragment, Viewport viewport);
Pipeline CreateGraphicsPipeline(Device::Device device, std::vector<DescriptorLayout::DescriptorLayout> layouts, Shader::Shader vertex, Shader::Shader geometry, Shader::Shader fragment, Viewport viewport);
void DestroyPipeline(Pipeline data);
} // namespace Pipeline
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif