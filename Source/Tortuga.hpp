#ifndef _TORTUGA
#define _TORTUGA

#include "./Tortuga/Console.hpp"

//Core
#include "./Tortuga/Core/Entity.hpp"
#include "./Tortuga/Core/System.hpp"

//Utils
#include "./Tortuga/Utils/IO.hpp"

//Internal Graphics API
#include "./Tortuga/Graphics/Vulkan/ErrorCheck.hpp"
#include "./Tortuga/Graphics/Vulkan/Instance.hpp"
#include "./Tortuga/Graphics/Vulkan/Device.hpp"
#include "./Tortuga/Graphics/Vulkan/Window.hpp"
#include "./Tortuga/Graphics/Vulkan/Swapchain.hpp"
#include "./Tortuga/Graphics/Vulkan/Shader.hpp"
#include "./Tortuga/Graphics/Vulkan/Image.hpp"
#include "./Tortuga/Graphics/Vulkan/Fence.hpp"
#include "./Tortuga/Graphics/Vulkan/Buffer.hpp"
#include "./Tortuga/Graphics/Vulkan/CommandPool.hpp"
#include "./Tortuga/Graphics/Vulkan/Command.hpp"
#include "./Tortuga/Graphics/Vulkan/DescriptorLayout.hpp"
#include "./Tortuga/Graphics/Vulkan/DescriptorPool.hpp"
#include "./Tortuga/Graphics/Vulkan/DescriptorSets.hpp"
#include "./Tortuga/Graphics/Vulkan/Pipeline.hpp"
#include "./Tortuga/Graphics/Vulkan/Renderer.hpp"
#include "./Tortuga/Graphics/Screen.hpp"
#include "./Tortuga/Graphics/Vertex.hpp"
#include "./Tortuga/Graphics/Mesh.hpp"

//Systems
#include "./Tortuga/Systems/Example.hpp"

#endif
