#ifndef _APPLICATION
#define _APPLICATION

#include "Core.h"
#include "Console.h"

#include "Graphics/VulkanAPI/DataStructures.h"
#include "Graphics/VulkanAPI/Vulkan.h"
#include "Graphics/VulkanAPI/Device.h"
#include "Graphics/VulkanAPI/Swapchain.h"

#include "Graphics/Window.h"

namespace Tortuga
{
void InitializeEngine(const char *applicationName);
void MainLoop();
void DestroyEngine();
}; // namespace Tortuga

#endif