#ifndef _VULKAN_SEMAPHORE
#define _VULKAN_SEMAPHORE

#include "./VulkanDevice.h"
#include "./VulkanErrorHandler.h"

namespace Tortuga {
  namespace Graphics {
    struct VulkanSemaphore {
      VkSemaphore Seamphore;
      VkDevice VirtualDevice;
    };

    VulkanSemaphore CreateSemaphore(VulkanDevice device);
    void DestroySemaphore(VulkanSemaphore data);
  }
}

#endif