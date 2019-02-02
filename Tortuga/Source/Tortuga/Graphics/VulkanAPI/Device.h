#ifndef _DEVICE
#define _DEVICE

#include "../../Core.h"
#include "../../Core.h"
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
  std::string _deviceName;
  uint32_t _vendorIdentifier;

  bool CheckDeviceSupport(VkPhysicalDevice device);
  bool CheckSwapchainSupport(SwapChainSupportDetails details);
  bool CheckExtensionSupport(VkPhysicalDevice device, std::vector<const char *> extensions);
  QueueFamilyIndices FindFamilyQueues(VkPhysicalDevice device, VkSurfaceKHR surface);
  SwapChainSupportDetails GetSwapchainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

public:
  Device(VkPhysicalDevice physicalDevice, std::vector<const char *> validationLayers, VkSurfaceKHR surface);
  ~Device();

  QueueFamilyIndices &GetQueueFamilyIndices() { return _familyQueues; }
  SwapChainSupportDetails &GetSwapchainSupportDetails() { return _swapchainDetails; }
  VkSurfaceKHR &GetSurface() { return _surface; }

  VkPhysicalDevice GetPhysicalDevice() { return _physicalDevice; }
  VkDevice GetVirtualDevice() { return _device; }
  VkQueue GetGraphicsQueue() { return _graphicsQueue; }
  VkQueue GetPresentQueue() { return _presentQueue; }
  bool IsReady() { return _isReady; }
  std::string GetDeviceName() { return _deviceName; }
  uint32_t GetVendorIdentifier() { return _vendorIdentifier; }
};
}; // namespace Tortuga

#endif