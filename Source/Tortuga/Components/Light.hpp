#ifndef _LIGHT
#define _LIGHT

#include <glm/glm.hpp>

#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Core/Engine.hpp"

namespace Tortuga
{
namespace Component
{
enum LightType
{
  Point = 0,
  Directional = 1
};
struct LightData
{
  glm::vec4 Position;
  glm::vec4 Rotation;
  uint Type;
  float Intensity;
  float Range;
};
struct Light
{
  bool IsEnabled;
  bool IsStatic;
  bool IsProcessed;
  LightData Data;
  Graphics::Vulkan::Buffer::Buffer Staging;
  Graphics::Vulkan::Buffer::Buffer Buffer;
  Graphics::Vulkan::CommandPool::CommandPool CommandPool;
  Graphics::Vulkan::Command::Command Command;

  void UpdateTransform(glm::vec4 position, glm::vec4 rotation);
  void SetStatic(bool isStatic);
  void SetActive(bool isActive);

  Light();
  ~Light();
};
} // namespace Component
} // namespace Tortuga

#endif