#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include "../Core/ECS/System.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/DisplaySurface.hpp"

namespace Tortuga
{
namespace Systems
{
class Rendering : public Core::ECS::System
{
private:
  Graphics::Vulkan::Instance::Instance VulkanInstance;
  Graphics::DisplaySurface::DisplaySurface DisplaySurface;
public:
  Rendering();
  ~Rendering();

  void Update();
};
} // namespace System
} // namespace Tortuga

#endif