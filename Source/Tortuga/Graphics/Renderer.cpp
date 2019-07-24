#include "./Renderer.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Renderer
{
Renderer Create(
    Vulkan::Instance::Instance vulkan,
    Vulkan::Device::Device device,
    uint32_t renderWidth,
    uint32_t renderHeight,
    std::vector<Vulkan::DescriptorLayout::DescriptorLayout> layouts)
{
    Renderer data = {};
    data.Width = renderWidth;
    data.Width = renderHeight;
    data.ImageSizeBytes = sizeof(float) * renderWidth * renderHeight * 4;
    data.Queues = device.Queues.Compute;

    std::vector<Vulkan::DescriptorLayout::Binding> bindings(1);
    {
        bindings[0] = {};
        bindings[0].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[0].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
        bindings[0].DescriptorCount = 1;
    }
    data.DescriptorLayout = Vulkan::DescriptorLayout::Create(device, bindings);
    layouts.push_back(data.DescriptorLayout);
    data.DescriptorPool = Vulkan::DescriptorPool::Create(device, data.DescriptorLayout);
    data.DescriptorSet = Vulkan::DescriptorSets::Create(device, data.DescriptorPool, {data.DescriptorLayout});

    auto shaderCode = Utils::IO::GetFileContents("Shaders/ray-marching.comp");
    auto compiledShader = Vulkan::Shader::CompileShader(vulkan, Vulkan::Shader::COMPUTE, shaderCode);
    data.Shader = Vulkan::Shader::Create(device, compiledShader);
    auto pipelineCache = Utils::IO::GetFileContents("Shaders/ray-marching.comp.cache");
    data.Pipeline = Vulkan::Pipeline::CreateComputePipeline(device, data.Shader, pipelineCache, layouts);
    auto newpipelineCache = Vulkan::Pipeline::GetCacheData(data.Pipeline);
    if (pipelineCache != newpipelineCache)
        Utils::IO::SetFileContents("Shaders/ray-marching.comp.cache", newpipelineCache);

    data.Image = Vulkan::Image::Create(device, renderWidth, renderHeight);
    data.Buffer = Vulkan::Buffer::Create(device, data.ImageSizeBytes, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    Vulkan::DescriptorSets::UpdateDescriptorSet(data.DescriptorSet, 0, {data.Buffer});

    data.CommandPool = Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
    data.Command = Vulkan::Command::Create(device, data.CommandPool, Vulkan::Command::PRIMARY);

    return data;
}
void Destroy(Renderer data)
{
    Vulkan::CommandPool::Destroy(data.CommandPool);
    Vulkan::Buffer::Destroy(data.Buffer);
    Vulkan::Image::Destroy(data.Image);
    Vulkan::Shader::Destroy(data.Shader);
    Vulkan::Pipeline::DestroyPipeline(data.Pipeline);
    Vulkan::DescriptorPool::Destroy(data.DescriptorPool);
    Vulkan::DescriptorLayout::Destroy(data.DescriptorLayout);
}
void Render(Renderer &data)
{
    Vulkan::Command::Submit({data.Command}, data.Queues[0]);
    Vulkan::ErrorCheck::Callback(vkQueueWaitIdle(data.Queues[0]));
}

void BindDescriptorSets(Renderer &data, std::vector<Vulkan::DescriptorSets::DescriptorSets> descriptorSets)
{
    descriptorSets.push_back(data.DescriptorSet);
    Vulkan::Command::Begin(data.Command, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
    Vulkan::Command::BindPipeline(data.Command, VK_PIPELINE_BIND_POINT_COMPUTE, data.Pipeline, descriptorSets);
    Vulkan::Command::Compute(data.Command, data.Image.Width / 8, data.Image.Height / 8, 1);
    Vulkan::Command::TransferImageLayout(data.Command, data.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Vulkan::Command::BufferToImage(data.Command, data.Buffer, data.Image, {0, 0}, {data.Width, data.Height});
    Vulkan::Command::TransferImageLayout(data.Command, data.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    Vulkan::Command::End(data.Command);
}
} // namespace Renderer
} // namespace Graphics
} // namespace Tortuga