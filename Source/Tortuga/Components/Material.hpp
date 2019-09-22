#ifndef _MATERIAL
#define _MATERIAL

#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/Image.hpp"
#include "../Graphics/Vulkan/ImageView.hpp"
#include "../Graphics/Vulkan/Sampler.hpp"
#include "../Graphics/Image.hpp"

namespace Tortuga
{
namespace Component
{
struct Material
{
  Graphics::Image Albedo = Graphics::Image();
};
} // namespace Component
} // namespace Tortuga

#endif