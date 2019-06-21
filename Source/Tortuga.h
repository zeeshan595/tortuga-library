#ifndef _TORTUGA
#define _TORTUGA

//External Libraries
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

//Core
#include "Tortuga/Core/Entity.h"
#include "Tortuga/Core/Environment.h"
#include "Tortuga/Core/System.h"
#include "Tortuga/Core/Engine.h"

//Systems
#include "Tortuga/Systems/MultiGPURenderingSystem.h"

//Utils
#include "Tortuga/Utils/InputOutput.h"

//Internal Graphics API
#include "Tortuga/Console.h"
#include "Tortuga/Graphics/Window.h"
#include "Tortuga/Graphics/VulkanErrorHandler.h"
#include "Tortuga/Graphics/VulkanInstance.h"
#include "Tortuga/Graphics/VulkanDevice.h"
#include "Tortuga/Graphics/VulkanSwapchain.h"
#include "Tortuga/Graphics/VulkanImage.h"
#include "Tortuga/Graphics/VulkanShader.h"
#include "Tortuga/Graphics/VulkanPipeline.h"
#include "Tortuga/Graphics/VulkanBuffer.h"
#include "Tortuga/Graphics/VulkanCommand.h"
#include "Tortuga/Graphics/VulkanFence.h"

#endif
