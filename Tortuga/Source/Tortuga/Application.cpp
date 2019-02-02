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
    _pipeline = new Pipeline(_swapchain, _applicationDir + "/Shaders/simple.vert.spv", _applicationDir + "/Shaders/simple.frag.spv");

    auto imageViews = _swapchain->GetSwapchainImageViews();
    _frameBuffers.resize(imageViews.size());
    for (uint32_t i = 0; i < imageViews.size(); i++)
        _frameBuffers[i] = new Framebuffer(_pipeline, { imageViews[i] });

    _vertexBuffer = new Buffer(device, vertices.size() * sizeof(vertices[0]), Buffer::BufferType::Vertex, Buffer::StorageType::DeviceCopy);
    _vertexBuffer->UpdateData(vertices);

    _indexBuffer = new Buffer(device, indices.size() * sizeof(indices[0]), Buffer::BufferType::Index, Buffer::StorageType::DeviceCopy);
    _indexBuffer->UpdateData(indices);

    _commandBuffer = new CommandBuffer(_pipeline, _frameBuffers);
    _commandBuffer->SetupDrawCall(_vertexBuffer, _indexBuffer, indices.size());
}

void Application::Destroy()
{
    delete _commandBuffer;

    delete _indexBuffer;
    delete _vertexBuffer;

    for (uint32_t i = 0; i < _frameBuffers.size(); i++)
        delete _frameBuffers[i];

    delete _pipeline;
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
        _commandBuffer->Render();
        SDL_Delay(1);
        i++;
        if (i > 5000) {
            break;
        }
    }
}
} // namespace Tortuga