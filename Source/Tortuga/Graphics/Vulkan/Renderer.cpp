#include "./Renderer.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace Vulkan
{
namespace Renderer
{
struct Options
{
    uint32_t offsetX;
    uint32_t offsetY;
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

void SetupBufferOptions(Device::Device device, Renderer data)
{
    const auto bufferOptionsStaging = Buffer::CreateHostSrc(device, sizeof(Options));
    const auto commandPool = CommandPool::Create(device, device.QueueFamilies.Transfer.Index);
    const auto command = Command::Create(device, commandPool, Command::PRIMARY);

    Options options = {};
    {
        options.offsetX = 0;
        options.offsetY = 0;
        options.width = data.Width;
        options.height = data.Height;
    }
    Buffer::SetData(bufferOptionsStaging, &options, sizeof(Options));

    Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Command::CopyBuffer(command, bufferOptionsStaging, data.BufferOptions);
    Command::End(command);

    Command::Submit({command}, device.Queues.Transfer[0]);
    Device::WaitForQueue(device.Queues.Transfer[0]);

    CommandPool::Destroy(commandPool);
    Buffer::Destroy(bufferOptionsStaging);
}

Renderer Create(
    Instance::Instance vulkan,
    Device::Device device,
    uint32_t renderWidth,
    uint32_t renderHeight,
    std::vector<DescriptorLayout::DescriptorLayout> layouts)
{
    if (renderWidth % 8 != 0 || renderHeight % 8 != 0)
    {
        Console::Error("Failed to create renderer, width and height must be divisible by shader 'LOCAL_INVOCATION'");
        return {};
    }

    Renderer data = {};
    data.Width = renderWidth;
    data.Height = renderHeight;
    data.ImageSizeBytes = sizeof(float) * renderWidth * renderHeight * 4;
    data.Queues = device.Queues.Compute;

    std::vector<DescriptorLayout::Binding> bindings(2);
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
    data.DescriptorLayout = DescriptorLayout::Create(device, bindings);
    layouts.push_back(data.DescriptorLayout);
    data.DescriptorPool = DescriptorPool::Create(device, data.DescriptorLayout);
    data.DescriptorSet = DescriptorSets::Create(device, data.DescriptorPool, {data.DescriptorLayout});

    auto shaderCode = Utils::IO::GetFileContents("Shaders/ray-marching.comp");
    auto compiledShader = Shader::CompileShader(vulkan, Shader::COMPUTE, shaderCode);
    data.Shader = Shader::Create(device, compiledShader);
    auto pipelineCache = Utils::IO::GetFileContents("Shaders/ray-marching.comp.cache");
    data.Pipeline = Pipeline::CreateComputePipeline(device, data.Shader, pipelineCache, layouts);
    auto newpipelineCache = Pipeline::GetCacheData(data.Pipeline);
    if (pipelineCache != newpipelineCache)
        Utils::IO::SetFileContents("Shaders/ray-marching.comp.cache", newpipelineCache);

    data.Image = Image::Create(device, renderWidth, renderHeight);
    data.Buffer = Buffer::CreateDeviceOnlySrc(device, data.ImageSizeBytes);
    data.BufferOptions = Buffer::CreateDeviceOnlyDest(device, sizeof(Options));
    data.CommandPool = CommandPool::Create(device, device.QueueFamilies.Compute.Index);
    data.Command = Command::Create(device, data.CommandPool, Command::PRIMARY);

    SetupBufferOptions(device, data);

    return data;
}
void Destroy(Renderer data)
{
    CommandPool::Destroy(data.CommandPool);
    Buffer::Destroy(data.Buffer);
    Buffer::Destroy(data.BufferOptions);
    Image::Destroy(data.Image);
    Pipeline::DestroyPipeline(data.Pipeline);
    Shader::Destroy(data.Shader);
    DescriptorPool::Destroy(data.DescriptorPool);
    DescriptorLayout::Destroy(data.DescriptorLayout);
}
void Render(Renderer data)
{
    Command::Submit({data.Command}, data.Queues[0]);
    ErrorCheck::Callback(vkQueueWaitIdle(data.Queues[0]));
}

void BindDescriptorSets(Renderer data, std::vector<DescriptorSets::DescriptorSets> descriptorSets)
{
    descriptorSets.push_back(data.DescriptorSet);
    DescriptorSets::UpdateDescriptorSets(data.DescriptorSet, 0, {data.Buffer, data.BufferOptions});
    Command::Begin(data.Command, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
    Command::BindPipeline(data.Command, VK_PIPELINE_BIND_POINT_COMPUTE, data.Pipeline, descriptorSets);
    Command::Compute(data.Command, data.Image.Width / 8, data.Image.Height / 8, 1);
    Command::TransferImageLayout(data.Command, data.Image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Command::BufferToImage(data.Command, data.Buffer, data.Image, {0, 0}, {data.Width, data.Height});
    Command::TransferImageLayout(data.Command, data.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    Command::End(data.Command);
}
void RenderThread(
    Device::Device device,
    Renderer renderer,
    Swapchain::Swapchain swapchain,
    Command::Command command,
    std::future<void> cancellation)
{
    while (cancellation.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        Render(renderer);
        const auto swapchainIndex = Swapchain::AquireNextImage(swapchain);
        const auto swapchainImage = Swapchain::GetImage(swapchain, swapchainIndex);

        //Record Command
        {
            Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
            Command::TransferImageLayout(command, swapchainImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
            Command::BlitImage(command, renderer.Image, swapchainImage, {renderer.Width, renderer.Height}, {0, 0}, {0, 0});
            Command::TransferImageLayout(command, swapchainImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
            Command::End(command);
        }

        //Copy image to swapchain
        Command::Submit({command}, device.Queues.Graphics[0]);
        Device::WaitForQueue(device.Queues.Graphics[0]);
        Swapchain::PresentImage(swapchain, swapchainIndex, device.Queues.Present);
        Device::WaitForQueue(device.Queues.Present);
    }
}
} // namespace Renderer
} // namespace Vulkan
} // namespace Graphics
} // namespace Tortuga