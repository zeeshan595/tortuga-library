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
#include "../Core/Screen.hpp"
#include "../Core/Entity.hpp"
#include "../Console.hpp"

#include "../Components/Mesh.hpp"
#include "../Components/Transform.hpp"

namespace Tortuga
{
namespace Systems
{
class Rendering : public Core::System
{
public:
  void Created()
  {
    //todo: create geometry pipeline
    //todo: create image object where rendered image will be stored
    //todo: create ray marching pipeline
  }
  void Update()
  {
    //todo: run all meshes through geometry pipeline

    auto entities = Core::Entity::GetAllEntities();
    for (auto entity : entities)
    {
      auto mesh = entity->GetComponent<Component::Mesh>();

    }

    //todo: Wait for all transformations to finish 
    //todo: start ray marching renderer pipeline
    //todo: make sure correct swapchain is bound
    //todo: present the image
  }
  void Destroyed()
  {
  }
};
} // namespace Systems
} // namespace Tortuga

#endif