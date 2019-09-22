#ifndef _ENGINE
#define _ENGINE

#include "../Graphics/Vulkan/Instance.hpp"

#include <vector>

namespace Tortuga
{
namespace Core
{
namespace Engine
{
Graphics::Vulkan::Instance::Instance GetVulkan();
std::vector<Graphics::Vulkan::Device::Device> GetAllDevices();
Graphics::Vulkan::Device::Device GetMainDevice();
uint32_t GetMainDeviceIndex();
}
} // namespace Core
} // namespace Tortuga

#endif