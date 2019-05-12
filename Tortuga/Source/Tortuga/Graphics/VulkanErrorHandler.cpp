#include "VulkanErrorHandler.h"

namespace Tortuga {
namespace Graphics {
void ErrorCheck(VkResult result) {
  switch (result) {
  case VK_SUCCESS:
    return;

  default:
    Console::Error("Vulkan Error: ", result);
    return;
  }

  Console::Fatal("Unknown vulkan error?");
}
} // namespace Graphics
} // namespace Tortuga