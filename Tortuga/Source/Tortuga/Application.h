#ifndef _APPLICATION
#define _APPLICATION

#include "Core.h"
#include "Console.h"

#include "Graphics/VulkanAPI/DataStructures.h"
#include "Graphics/VulkanAPI/Vulkan.h"

namespace Tortuga
{
void InitializeEngine(const char *applicationName);
void MainLoop();
void DestroyEngine();
}; // namespace Tortuga

#endif