#ifndef _VULKAN_API_PIPELINE_LAYOUT
#define _VULKAN_API_PIPELINE_LAYOUT

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "DescriptorSetLayout.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class PipelineLayout
{
private:
  Device *_device;
  VkPipelineLayout _pipelineLayout;

public:
  PipelineLayout(Device *device, std::vector<DescriptorSetLayout *> descriptorSetLayouts);
  ~PipelineLayout();

  VkPipelineLayout GetPipelineLayout() { return _pipelineLayout; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif