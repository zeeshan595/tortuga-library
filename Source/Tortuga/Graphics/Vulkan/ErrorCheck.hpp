#ifndef _VULKAN_ERROR_CHECK
#define _VULKAN_ERROR_CHECK

#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan.h>

#include "../../Console.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace ErrorCheck
{
void Callback(VkResult result)
{
  if (result != VK_SUCCESS)
  {
    Console::Fatal("invalid response from vulkan function: {0}", result);
  }
}
} // namespace ErrorCheck
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif