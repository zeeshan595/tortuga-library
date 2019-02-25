#include <Tortuga.h>

using namespace Tortuga;

const std::vector<Graphics::Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0};

std::string GetWorkingDirectory(std::string executablePath)
{
    uint32_t i = executablePath.find_last_of('/');
    return executablePath.substr(0, i);
}

int main(int argc, char **argv)
{
    std::string executablePath = argv[0];
    std::string workingDirectory = GetWorkingDirectory(executablePath);

    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    auto renderingEngine = Graphics::CreateRenderingEngine();
    auto window = Graphics::CreateWindow(
        renderingEngine,
        "Hello World",
        1024, 768,
        Graphics::WindowType::ResizeableWindowed);

    auto hardware = Graphics::CreateHardwareController(
        renderingEngine,
        window,
        {renderingEngine.Devices[0],
         renderingEngine.Devices[0]});

    auto renderpass = Graphics::CreateRenderPass(hardware);
    auto framebuffers = Graphics::CreateFrameBuffers(hardware, renderpass);
    auto renderer = Graphics::CreateRenderer(hardware, window, framebuffers, renderpass);

    //SCENE RENDERING START
    auto vertexShader = Graphics::CreateShaderFromFile(
        hardware,
        workingDirectory + "/Shaders/simple.vert.spv",
        Graphics::SHADER_TYPE_VERTEX);

    auto fragmentShader = Graphics::CreateShaderFromFile(
        hardware,
        workingDirectory + "/Shaders/simple.frag.spv",
        Graphics::SHADER_TYPE_FRAGMENT);
    auto pipeline = Graphics::CreatePipeline(hardware, renderpass, {vertexShader, fragmentShader});

    auto commandPool = Graphics::CreateCommandPool(hardware);
    auto commandBuffer = Graphics::CreateCommandBuffer(hardware, commandPool, Graphics::COMMAND_BUFFER_SECONDARY, 1);

    auto vertexBuffer = Graphics::CreateBuffer(
        hardware,
        Graphics::BUFFER_TYPE_VERTEX,
        sizeof(vertices[0]) * vertices.size());
    Graphics::UpdateBufferData(vertexBuffer, vertices);

    auto indexBuffer = Graphics::CreateBuffer(
        hardware,
        Graphics::BUFFER_TYPE_INDEX,
        sizeof(indices[0]) * indices.size());
    Graphics::UpdateBufferData(indexBuffer, indices);

    auto tempPool = Graphics::CreateCommandPool(hardware);
    auto tempBuffer = Graphics::CreateCommandBuffer(hardware, tempPool, Graphics::COMMAND_BUFFER_PRIMARY, 1);
    Graphics::BeginCommandBuffer(tempBuffer, 0);
    Graphics::CommandBufferUpdateBuffer(tempBuffer, 0, vertexBuffer);
    Graphics::CommandBufferUpdateBuffer(tempBuffer, 0, indexBuffer);
    Graphics::EndCommandBuffer(tempBuffer, 0);
    Graphics::CommandBufferSubmitCommands(tempBuffer);
    Graphics::DestroyCommandPool(tempPool);

    Graphics::BeginCommandBuffer(commandBuffer, 0, renderpass, 0);
    Graphics::BindCommandBufferPipeline(commandBuffer, 0, pipeline);
    Graphics::CommandBufferDraw(commandBuffer, 0, vertexBuffer, indexBuffer, indices.size());
    Graphics::EndCommandBuffer(commandBuffer, 0);

    //Record all Scene Info
    Graphics::SubmitCommands(renderer, {commandBuffer});
    //SCENE RENDERING END

    //Main loop
    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event) != 0)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                isRunning = false;
                break;
            }
        }

        Graphics::DrawFrame(renderer);
    }

    Graphics::DestroyRenderer(renderer);

    Graphics::DestroyBuffer(vertexBuffer);
    Graphics::DestroyBuffer(indexBuffer);
    Graphics::DestroyCommandPool(commandPool);

    Graphics::DestroyPipeline(pipeline);

    Graphics::DestroyFrameBuffers(framebuffers);
    Graphics::DestroyRenderPass(renderpass);

    Graphics::DestroyShader(vertexShader);
    Graphics::DestroyShader(fragmentShader);

    Graphics::DestroyWindow(window);
    Graphics::DestroyRenderingEngine(renderingEngine);

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}