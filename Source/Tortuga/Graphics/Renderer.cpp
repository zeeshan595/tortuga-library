#include "./Renderer.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Renderer
{
struct Options
{
    uint32_t width;
    uint32_t height;
};
struct Pixel
{
    float r;
    float g;
    float b;
    float a;
};

void SetupBufferOptions(Vulkan::Device::Device device, Renderer data)
{
    const auto bufferOptionsStaging = Vulkan::Buffer::Create(device, sizeof(Options), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    const auto commandPool = Vulkan::CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
    const auto command = Vulkan::Command::Create(device, commandPool, Vulkan::Command::PRIMARY);

    Options options = {};
    {
        options.width = data.Width;
        options.height = data.Height;
    }
    Vulkan::Buffer::SetData(bufferOptionsStaging, &options, sizeof(Options));

    Vulkan::Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Vulkan::Command::CopyBuffer(command, bufferOptionsStaging, data.BufferOptions);
    Vulkan::Command::End(command);

    Vulkan::Command::Submit({command}, device.Queues.Transfer[0]);
    Vulkan::Device::WaitForQueue(device.Queues.Transfer[0]);
    Vulkan::CommandPool::Destroy(commandPool);
    Vulkan::Buffer::Destroy(bufferOptionsStaging);
}

Renderer Create(
    Vulkan::Instance::Instance vulkan,
    Vulkan::Device::Device device,
    uint32_t renderWidth,
    uint32_t renderHeight,
    std::vector<Vulkan::DescriptorLayout::DescriptorLayout> layouts)
{
    Renderer data = {};
    data.Width = renderWidth;
    data.Height = renderHeight;
    data.ImageSizeBytes = sizeof(float) * renderWidth * renderHeight * 4;
    data.Queues = device.Queues.Compute;

    std::vector<Vulkan::DescriptorLayout::Binding> bindings(2);
    {
        bindings[0] = {};
        bindings[0].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[0].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
        bindings[0].DescriptorCount = 1;

        bindings[1] = {};
        bindings[1].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[1].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
        bindings[1].DescriptorCount = 1;
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
    data.BufferOptions = Vulkan::Buffer::Create(device, sizeof(Options), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    data.CommandPool = Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
    data.Command = Vulkan::Command::Create(device, data.CommandPool, Vulkan::Command::PRIMARY);

    SetupBufferOptions(device, data);

    return data;
}
void Destroy(Renderer data)
{
    Vulkan::CommandPool::Destroy(data.CommandPool);
    Vulkan::Buffer::Destroy(data.BufferOptions);
    Vulkan::Buffer::Destroy(data.Buffer);
    Vulkan::Image::Destroy(data.Image);
    Vulkan::Shader::Destroy(data.Shader);
    Vulkan::Pipeline::DestroyPipeline(data.Pipeline);
    Vulkan::DescriptorPool::Destroy(data.DescriptorPool);
    Vulkan::DescriptorLayout::Destroy(data.DescriptorLayout);
}
void Render(Renderer data)
{
    Vulkan::Command::Submit({data.Command}, data.Queues[0]);
    Vulkan::ErrorCheck::Callback(vkQueueWaitIdle(data.Queues[0]));
}

void BindDescriptorSets(Renderer data, std::vector<Vulkan::DescriptorSets::DescriptorSets> descriptorSets)
{
    descriptorSets.push_back(data.DescriptorSet);
    Vulkan::DescriptorSets::UpdateDescriptorSets(data.DescriptorSet, 0, {data.Buffer, data.BufferOptions});
    Vulkan::Command::Begin(data.Command, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
    Vulkan::Command::BindPipeline(data.Command, VK_PIPELINE_BIND_POINT_COMPUTE, data.Pipeline, descriptorSets);
    Vulkan::Command::Compute(data.Command, data.Image.Width / 8, data.Image.Height / 8, 1);
    Vulkan::Command::TransferImageLayout(data.Command, data.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Vulkan::Command::BufferToImage(data.Command, data.Buffer, data.Image, {0, 0}, {data.Width, data.Height});
    Vulkan::Command::TransferImageLayout(data.Command, data.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    Vulkan::Command::End(data.Command);
}
void RenderThread(
    Graphics::Vulkan::Device::Device device,
    Graphics::Renderer::Renderer renderer,
    Graphics::Vulkan::Swapchain::Swapchain swapchain,
    Graphics::Vulkan::Command::Command command,
    std::future<void> cancellation)
{
    while (cancellation.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        Graphics::Renderer::Render(renderer);
        const auto swapchainIndex = Graphics::Vulkan::Swapchain::AquireNextImage(swapchain);
        const auto swapchainImage = Graphics::Vulkan::Swapchain::GetImage(swapchain, swapchainIndex);

        //Record Command
        {
            Graphics::Vulkan::Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
            Graphics::Vulkan::Command::TransferImageLayout(command, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            Graphics::Vulkan::Command::BlitImage(command, renderer.Image, swapchainImage, {renderer.Width, renderer.Height}, {0, 0}, {0, 0});
            Graphics::Vulkan::Command::TransferImageLayout(command, swapchainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
            Graphics::Vulkan::Command::End(command);
        }

        //Copy image to swapchain
        Graphics::Vulkan::Command::Submit({command}, device.Queues.Graphics[0]);
        Graphics::Vulkan::Device::WaitForQueue(device.Queues.Graphics[0]);
        Graphics::Vulkan::Swapchain::PresentImage(swapchain, swapchainIndex, device.Queues.Present);
    }
}
} // namespace Renderer
} // namespace Graphics
} // namespace Tortuga