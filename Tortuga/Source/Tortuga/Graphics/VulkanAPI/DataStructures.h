#ifndef _DATA_STRUCTURES
#define _DATA_STRUCTURES

#include "../../Core.h"
#include "../../Console.h"

namespace Tortuga
{
namespace Graphics
{
namespace VulkanAPI
{
struct VulkanData
{
  std::vector<const char *> InstanceExtensions;
  std::vector<const char *> ValidationLayers;
  VkInstance Instance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT Debugger = VK_NULL_HANDLE;
};

struct QueueFamilyIndices
{
  std::optional<uint32_t> GraphicsFamily;
  std::optional<uint32_t> PresentFamily;

  static bool IsComplete(QueueFamilyIndices indices)
  {
    return indices.GraphicsFamily.has_value() &&
           indices.PresentFamily.has_value();
  }
};

struct DeviceData
{
  VkInstance Instance;
  QueueFamilyIndices QueueFamilies;
  VkPhysicalDeviceProperties Properties;
  VkPhysicalDeviceFeatures Features;
  VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
  VkDevice Device = VK_NULL_HANDLE;
  uint32_t Score;
  VkQueue PresentQueue = VK_NULL_HANDLE;
  VkQueue GraphicQueue = VK_NULL_HANDLE;
};

struct SurfaceData
{
  VkInstance Instance;
  VkDevice Device;
  VkSurfaceKHR Surface;
};

struct WindowData
{
  SDL_Window *Window;
  SurfaceData Surface;
};

struct VulkanImageData
{
  VkDevice Device;
  VkImage Image;
  VkDeviceMemory Memory;
  VkImageView ImageView;
};

struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR Capabilities;
  std::vector<VkSurfaceFormatKHR> Formats;
  std::vector<VkPresentModeKHR> PresentModes;
};

struct SwapchainData
{
  VkDevice Device = VK_NULL_HANDLE;
  SwapChainSupportDetails SupportDetails;
  VkSurfaceFormatKHR SurfaceFormat;
  VkPresentModeKHR PresentMode;
  VkExtent2D Extent;
  uint32_t ImageCount;
  VkSwapchainKHR Swapchain;
  std::vector<VkImage> Images;
  std::vector<VkImageView> ImageViews;
};

struct ShaderData
{
  VkDevice Device = VK_NULL_HANDLE;
  std::vector<char> Code;
  VkShaderModule Shader = VK_NULL_HANDLE;
  VkPipelineShaderStageCreateInfo StageInfo;
};

struct PipelineData
{
  VkDevice Device = VK_NULL_HANDLE;
  VkPipelineLayout Layout = VK_NULL_HANDLE;
  VkPipeline Pipeline;
};

struct RenderPassData
{
  VkDevice Device;
  VkRenderPass RenderPass;
};

struct FrameBufferData
{
  VkDevice Device;
  std::vector<VulkanImageData> Images;
  VkFramebuffer FrameBuffers;
  uint32_t Width;
  uint32_t Height;
};

struct CommandPoolData
{
  VkDevice Device;
  VkCommandPool Pool;
};

struct CommandBufferData
{
  VkDevice Device;
  std::vector<VkCommandBuffer> Buffer;
};

struct RendererData
{
  DeviceData Device;
  FrameBufferData FrameBuffers;
  RenderPassData RenderPass;

  CommandPoolData CommandPool;
  CommandBufferData CommandBuffer;
  std::vector<VkPipelineStageFlags> WaitStages;
  VkFence OnCompleteFence;
};

struct BufferData
{
  VkDevice Device;
  uint32_t BufferSize;
  VkBuffer Buffer;
  VkDeviceMemory Memory;
};

struct DescriptorLayoutData
{
  VkDevice Device;
  VkDescriptorSetLayout Layout;
};
struct DescriptorPoolData
{
  VkDevice Device;
  VkDescriptorPool Pool;
  VkDescriptorType Type;
  uint32_t DescriptorSize;
  uint32_t DescriptorSetSize;
};
struct DescriptorSetData
{
  VkDevice Device;
  uint32_t DescriptorSize;
  std::vector<VkDescriptorSet> DescriptorSet;
};
}; // namespace VulkanAPI
}; // namespace Graphics
}; // namespace Tortuga

#endif