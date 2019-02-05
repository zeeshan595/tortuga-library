#include "Application.h"

namespace Tortuga
{
void Application::Initialize(std::string path)
{
    this->_applicationPath = path;
    this->_applicationDir = path.substr(0, path.find_last_of("/"));

    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    _mainWindow = new Window(this->ApplicationName.c_str(), 1024, 768);
    _vulkan = new Vulkan(_mainWindow, this->ApplicationName.c_str());
    Device *device = _vulkan->GetDevices()[0];
    _swapchain = new Swapchain(device, _mainWindow->GetWidth(), _mainWindow->GetHeight());

    _descriptorSetLayouts = {new DescriptorSetLayout(device)};
    _pipelineLayout = new PipelineLayout(device, _descriptorSetLayouts);
    _renderPass = new RenderPass(device, _swapchain);
    _shader = new Shader(device, _applicationDir + "/Shaders/simple.vert.spv", _applicationDir + "/Shaders/simple.frag.spv");
    _pipeline = new Pipeline(device, _swapchain, _renderPass, _shader, _pipelineLayout);

    _frameBuffer = new FrameBuffer(device, _swapchain, _renderPass);

    //////////////////////

    _descriptorPool = new DescriptorPool(device, _swapchain->GetSwapchainRawImages().size());
    _descriptorSet = new DescriptorSet(device, _descriptorSetLayouts[0], _descriptorPool, 1);

    _vertexBuffer = new Buffer(device, vertices.size() * sizeof(vertices[0]), Buffer::BufferType::Vertex, StorageType::DeviceCopy);
    _vertexBuffer->UpdateData(vertices);

    _indexBuffer = new Buffer(device, indices.size() * sizeof(indices[0]), Buffer::BufferType::Index, StorageType::DeviceCopy);
    _indexBuffer->UpdateData(indices);
    _uniformBuffer = new Buffer(device, sizeof(ubo), Buffer::BufferType::Uniform, StorageType::DeviceOnly);
    _descriptorSet->UpdateDescriptorSet({_uniformBuffer}, sizeof(ubo));

    _commandPool = new CommandPool(device);

    _commandBuffer = new CommandBuffer(device, _commandPool, 1, false);
    _commandBuffer->BeginCommandBuffer(0, _renderPass, 0);
    _commandBuffer->BindPipeline(0, _pipeline);
    _commandBuffer->SetViewport(0, 0, 0, 1024, 768);
    _commandBuffer->SetScissor(0, 0, 0, 1024, 768);
    _commandBuffer->BindDescriptorSet(0, _pipelineLayout, _descriptorSet->GetDescriptorSets());
    _commandBuffer->CreateDrawCommand(0, _vertexBuffer, _indexBuffer, indices.size());
    _commandBuffer->EndCommandBuffer(0);

    _renderer = new Renderer(device, _swapchain, _renderPass, _frameBuffer);
    _renderer->RecordCommandBuffers({_commandBuffer});
}

void Application::Destroy()
{
    _renderer->WaitForGPUIdle();
    delete _commandBuffer;
    delete _commandPool;

    delete _indexBuffer;
    delete _vertexBuffer;
    delete _uniformBuffer;

    delete _renderer;

    delete _descriptorPool;

    delete _frameBuffer;

    delete _pipeline;
    delete _pipelineLayout;
    delete _shader;
    for (uint32_t i = 0; i < _descriptorSetLayouts.size(); i++)
        delete _descriptorSetLayouts[i];

    delete _renderPass;
    delete _swapchain;
    delete _vulkan;
    delete _mainWindow;
    SDL_Quit();
}

void Application::Run()
{
    int i = 0;
    while (true)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();
        SDL_Delay(1);
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        auto swapChainExtent = _swapchain->GetExtent2D();
        ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        _uniformBuffer->UpdateData(ubo);

        _renderer->RenderFrame();
        i++;
        if (i > 5000)
        {
            break;
        }
    }
}
} // namespace Tortuga