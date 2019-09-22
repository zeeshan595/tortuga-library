#ifndef _LIGHT
#define _LIGHT

#include <glm/glm.hpp>

#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSet.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"

namespace Tortuga
{
namespace Component
{
enum LightType
{
  POINT = 0,
  DIRECTIONAL = 1
};
struct Light
{
  const static uint32_t MAX_LIGHT_NUM = 16;
  bool IsEnabled;
  Graphics::Vulkan::Buffer::Buffer StagingLightBuffer;
  LightType Type;
  glm::vec4 Color;
  uint32_t Intensity;
  uint32_t Range;

  Light()
  {
    IsEnabled = true;
    Type = LightType::POINT;
    Color = glm::vec4(1, 1, 1, 1);
    Intensity = 1.0f;
    Range = 10.0f;
  }
};
} // namespace Component
} // namespace Tortuga

#endif