#ifndef _RENDER_PASS
#define _RENDER_PASS

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/RenderPass.h"

#include "Window.h"

namespace Tortuga
{
namespace Graphics
{
struct RenderPass
{
  std::vector<VulkanAPI::RenderPassData> VulkanRenderPass;
};

RenderPass CreateRenderPass(Window window);
void DestroyRenderPass(RenderPass data);
}; // namespace Graphics
}; // namespace Tortuga

#endif