#ifndef _DESCRIPTOR_SET_LAYOUT
#define _DESCRIPTOR_SET_LAYOUT

#include "../../Core.h"
#include "../../Console.h"
#include "Device.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
class DescriptorSetLayout
{
public:
  enum LayoutType
  {
    Buffer,
    Image
  };
  enum ShaderType
  {
    Vertex,
    Fragment
  };
  struct DescriptorType
  {
    LayoutType layout;
    ShaderType shader;
  };

private:
  Device *_device;
  VkDescriptorSetLayout _descriptorSetLayout;
  std::vector<DescriptorType> _layouts;

public:
  DescriptorSetLayout(Device *device, std::vector<DescriptorType> layouts);
  ~DescriptorSetLayout();

  VkDescriptorSetLayout GetDescriptorSetLayout() { return _descriptorSetLayout; }
  uint32_t GetSize() { return _layouts.size(); }
  std::vector<DescriptorType> GetLyouts() { return _layouts; }
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif