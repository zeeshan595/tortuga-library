#ifndef _SURFACE_INTERFACE
#define _SURFACE_INTERFACE

#include <vulkan/vulkan.h>

namespace Tortuga
{
namespace Graphics
{
namespace Surface
{
class SurfaceInterface
{
public:
  SurfaceInterface(){}
  virtual ~SurfaceInterface(){}

  virtual VkSurfaceKHR CreateSurface(VkInstance instance)
  {
    return VK_NULL_HANDLE;
  }
  virtual void Dispatch() {}
  static std::vector<const char *> GetVulkanExtensions();
  static bool HasPresentSupport(VkPhysicalDevice device, uint32_t familyIndex);
};
} // namespace Surface
} // namespace Graphics
} // namespace Tortuga

#endif