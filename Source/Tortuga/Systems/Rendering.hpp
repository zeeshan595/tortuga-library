#ifndef _EXAMPLE_SYSTEM
#define _EXAMPLE_SYSTEM

#include "../Graphics/Vulkan/DescriptorLayout.hpp"
#include "../Graphics/Vulkan/DescriptorPool.hpp"
#include "../Graphics/Vulkan/DescriptorSets.hpp"
#include "../Graphics/Vulkan/Pipeline.hpp"
#include "../Graphics/Vulkan/Buffer.hpp"
#include "../Graphics/Vulkan/CommandPool.hpp"
#include "../Graphics/Vulkan/Command.hpp"
#include "../Graphics/Vulkan/Instance.hpp"
#include "../Graphics/Vulkan/Window.hpp"

#include "../Core/System.hpp"
#include "../Components/Mesh.hpp"
#include "../Console.hpp"

namespace Tortuga
{
namespace Systems
{
class Rendering : public Core::System
{
public:
  std::vector<ComponentData<Component::Mesh> *> MeshData;

  void Start()
  {

  }
  void Update()
  {
  }
  void End()
  {
  }

  void FetchData()
  {
    this->MeshData = GetData<Component::Mesh>();
  }
};
} // namespace Systems
} // namespace Tortuga

#endif