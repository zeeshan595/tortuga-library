#ifndef _VULKAN_RENDERER
#define _VULKAN_RENDERER

#include <future>

#include "../../Utils/IO.hpp"
#include "./ErrorCheck.hpp"
#include "./Instance.hpp"
#include "./Device.hpp"
#include "./DescriptorLayout.hpp"
#include "./DescriptorPool.hpp"
#include "./DescriptorSets.hpp"
#include "./Shader.hpp"
#include "./Pipeline.hpp"
#include "./Buffer.hpp"
#include "./CommandPool.hpp"
#include "./Command.hpp"
#include "./Swapchain.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{

namespace Renderer
{
struct Renderer
{
    std::vector<VkQueue> Queues;
    CommandPool::CommandPool CommandPool;
    Command::Command Command;
    DescriptorLayout::DescriptorLayout DescriptorLayout;
    DescriptorPool::DescriptorPool DescriptorPool;
    DescriptorSets::DescriptorSets DescriptorSet;
    Shader::Shader Shader;
    Pipeline::Pipeline Pipeline;
    Buffer::Buffer Buffer;
    Buffer::Buffer BufferOptions;
    Image::Image Image;
    uint32_t Width;
    uint32_t Height;
    uint32_t ImageSizeBytes;
};

Renderer Create(Instance::Instance vulkan, Device::Device device, uint32_t renderWidth, uint32_t renderHeight, std::vector<DescriptorLayout::DescriptorLayout> layouts = {});
void Destroy(Renderer data);
void Render(Renderer data);
void BindDescriptorSets(Renderer data, std::vector<DescriptorSets::DescriptorSets> descriptorSets = {});
void RenderThread(
    Device::Device device,
    Renderer renderer,
    Swapchain::Swapchain swapchain,
    Command::Command command,
    std::future<void> cancellation);
} // namespace Renderer
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga

#endif