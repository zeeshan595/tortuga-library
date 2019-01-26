#ifndef _DEVICE
#define _DEVICE

#include "../Core.h"
#include "../Console.h"
#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

namespace Tortuga
{
class Device
{
private:
  bool _isReady = false;
  QueueFamilyIndices _familyQueues;
  SwapChainSupportDetails _swapchainDetails;
  VkSurfaceKHR _surface;
  VkPhysicalDevice _physicalDevice;
  VkDevice _device;
  VkQueue _graphicsQueue;
  VkQueue _presentQueue;

  bool CheckDeviceSupport(VkPhysicalDevice device);
  bool CheckSwapchainSupport(SwapChainSupportDetails details);
  bool CheckExtensionSupport(VkPhysicalDevice device, std::vector<const char *> extensions);
  QueueFamilyIndices FindFamilyQueues(VkPhysicalDevice device, VkSurfaceKHR surface);
  SwapChainSupportDetails GetSwapchainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

public:
  Device(VkPhysicalDevice physicalDevice, std::vector<const char *> validationLayers, VkSurfaceKHR surface);
  ~Device();

  QueueFamilyIndices GetQueueFamilyIndices() { return _familyQueues; }
  SwapChainSupportDetails GetSwapchainSupportDetails() { return _swapchainDetails; }
  VkSurfaceKHR GetSurface() { return _surface; }

  const VkPhysicalDevice GetPhysicalDevice() { return _physicalDevice; }
  const VkDevice GetVirtualDevice() { return _device; }
  const VkQueue GetGraphicsQueue() { return _graphicsQueue; }
  const VkQueue GetPresentQueue() { return _presentQueue; }
  const bool IsReady() { return _isReady; }
};
}; // namespace Tortuga

#endif