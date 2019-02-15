#ifndef _DATA_STRUCTURES
#define _DATA_STRUCTURES

#include "../../Core.h"

struct VulkanData
{
  std::vector<const char *> InstanceExtensions;
  std::vector<const char *> ValidationLayers;
  VkInstance Instance;
  VkDebugUtilsMessengerEXT Debugger;
};

#endif