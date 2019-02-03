#ifndef _PIPELINE_LAYOUT
#define _PIPELINE_LAYOUT

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"
#include "DescriptorSetLayout.h"

namespace Tortuga
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
}; // namespace Tortuga

#endif