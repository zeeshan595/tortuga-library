#ifndef _TORTUGA_VULKAN
#define _TORTUGA_VULKAN

#include <vulkan.h>
#include <vector>
#include <stdexcept>
#include <iostream>

namespace Tortuga
{
namespace Core
{
namespace Graphics
{
class Vulkan
{
  private:
    VkInstance _instance;

  public:
    Vulkan(std::vector<const char*> extensions);
    ~Vulkan();

    VkInstance GetInstance();
};
}; // namespace Graphics
}; // namespace Core
}; // namespace Tortuga

#endif