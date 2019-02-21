#include <Tortuga.h>

using namespace Tortuga;

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
        renderingEngine.Devices[renderingEngine.MainDeviceIndex],
        "Hello World",
        1024, 768,
        Graphics::WindowType::ResizeableWindowed);

    auto hardware = Graphics::CreateHardwareController(
        renderingEngine,
        window,
        {renderingEngine.Devices[0], renderingEngine.Devices[0]});

    auto renderpass = Graphics::CreateRenderPass(hardware);
    auto framebuffers = Graphics::CreateFrameBuffers(hardware, renderpass);

    auto vertexShader = Graphics::CreateShaderFromFile(
        hardware,
        workingDirectory + "/Shaders/simple.vert.spv",
        Graphics::ShaderType::Vertex);

    auto fragmentShader = Graphics::CreateShaderFromFile(
        hardware,
        workingDirectory + "/Shaders/simple.frag.spv",
        Graphics::ShaderType::Fragment);
    auto pipeline = Graphics::CreatePipeline(hardware, renderpass, {vertexShader, fragmentShader});

    auto commandPool = Graphics::CreateCommandPool(hardware);
    auto commandBuffer = Graphics::CreateCommandBuffer(hardware, commandPool, Graphics::CommandBufferLevel::CommandBufferSecondary, 1);

    auto renderer = Graphics::CreateRenderer(hardware, window, framebuffers, renderpass);

    Graphics::BeginCommandBuffer(commandBuffer, 0, renderpass, 0);
    Graphics::BindCommandBufferPipeline(commandBuffer, 0, pipeline);
    Graphics::CommandBufferDrawExample(commandBuffer, 0);
    Graphics::EndCommandBuffer(commandBuffer, 0);

    Graphics::SubmitCommands(renderer, {commandBuffer});

    Graphics::DrawFrame(renderer);

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
            switch (event.type)
            {
            case SDL_KEYDOWN:
                Graphics::DrawFrame(renderer);
                break;
            }
        }
    }

    Graphics::DestroyRenderer(renderer);

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