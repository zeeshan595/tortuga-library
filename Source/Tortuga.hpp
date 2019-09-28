#ifndef _TORTUGA
#define _TORTUGA

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC

//debug console
#include "./Tortuga/Console.hpp"

//Utils
#include "./Tortuga/Utils/IO.hpp"

//Vulkan library
#include "./Tortuga/Graphics/Vulkan/ErrorCheck.hpp"
#include "./Tortuga/Graphics/Vulkan/Instance.hpp"
#include "./Tortuga/Graphics/Vulkan/Device.hpp"
#include "./Tortuga/Graphics/Vulkan/Swapchain.hpp"
#include "./Tortuga/Graphics/Vulkan/Shader.hpp"
#include "./Tortuga/Graphics/Vulkan/Image.hpp"
#include "./Tortuga/Graphics/Vulkan/Fence.hpp"
#include "./Tortuga/Graphics/Vulkan/Semaphore.hpp"
#include "./Tortuga/Graphics/Vulkan/Buffer.hpp"
#include "./Tortuga/Graphics/Vulkan/CommandPool.hpp"
#include "./Tortuga/Graphics/Vulkan/Command.hpp"
#include "./Tortuga/Graphics/Vulkan/DescriptorLayout.hpp"
#include "./Tortuga/Graphics/Vulkan/DescriptorPool.hpp"
#include "./Tortuga/Graphics/Vulkan/DescriptorSet.hpp"
#include "./Tortuga/Graphics/Vulkan/Pipeline.hpp"
#include "./Tortuga/Graphics/Pixel.hpp"
#include "./Tortuga/Graphics/Image.hpp"
#include "./Tortuga/Graphics/Vertex.hpp"
#include "./Tortuga/Graphics/DisplaySurface.hpp"

//Core
#include "./Tortuga/Core/GUID.hpp"


#endif
