#ifndef _VULKAN_ERROR_HANDLER
#define _VULKAN_ERROR_HANDLER

#include "../Console.h"
#include <vulkan/vulkan.h>

namespace Tortuga {
namespace Graphics {
void ErrorCheck(VkResult result);
} // namespace Graphics
} // namespace Tortuga

#endif