#ifndef _ENGINE
#define _ENGINE

#include "../Graphics/Vulkan/Instance.hpp"

namespace Tortuga
{
namespace Core
{
namespace Engine
{
Graphics::Vulkan::Instance::Instance GetVulkan();
Graphics::Vulkan::Device::Device GetMainDevice();
uint32_t GetMainDeviceIndex();
}
} // namespace Core
} // namespace Tortuga

#endif