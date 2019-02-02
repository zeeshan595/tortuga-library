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
    _renderPass = new RenderPass(device, _swapchain);
    _pipeline = new Pipeline(device, _renderPass, _swapchain, _applicationDir + "/Shaders/simple.vert.spv", _applicationDir + "/Shaders/simple.frag.spv");

    auto imageViews = _swapchain->GetSwapchainImageViews();
    _frameBuffers.resize(imageViews.size());
    for (uint32_t i = 0; i < imageViews.size(); i++)
        _frameBuffers[i] = new FrameBuffer(device, _swapchain, _renderPass, {imageViews[i]});

    _vertexBuffer = new Buffer(device, vertices.size() * sizeof(vertices[0]), Buffer::BufferType::Vertex, Buffer::StorageType::DeviceCopy);
    _vertexBuffer->UpdateData(vertices);

    _indexBuffer = new Buffer(device, indices.size() * sizeof(indices[0]), Buffer::BufferType::Index, Buffer::StorageType::DeviceCopy);
    _indexBuffer->UpdateData(indices);

    _renderer = new Renderer(device, _pipeline, _renderPass, _frameBuffers);

    _commandPool = new CommandPool(device);

    _commandBuffer = new CommandBuffer(device, _commandPool, 1, false);
    _commandBuffer->CreateDrawCommand(0, _pipeline, _renderPass, 0, _vertexBuffer, _indexBuffer, indices.size());

    _renderer->RecordCommandBuffers({_commandBuffer});
}

void Application::Destroy()
{
    _renderer->WaitGPUIdle();
    delete _commandBuffer;
    delete _commandPool;

    delete _indexBuffer;
    delete _vertexBuffer;

    delete _renderer;

    for (uint32_t i = 0; i < _frameBuffers.size(); i++)
        delete _frameBuffers[i];

    delete _pipeline;
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
        _renderer->RenderFrame();
        SDL_Delay(1);
        i++;
        if (i > 5000)
        {
            break;
        }
    }
}
} // namespace Tortuga