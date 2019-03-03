#ifndef _SYSTEMS_RENDERING
#define _SYSTEMS_RENDERING

#include "../Core.h"
#include "../SystemController.h"

namespace Tortuga
{
struct Temp
{
  glm::vec3 color = {1.0, 0.0, 0.0};
};
class RenderingSystem : public System
{
private:
  std::string ApplicationDir;
  Graphics::RenderingEngine Engine;
  Graphics::Window Window;
  Graphics::HardwareController Hardware;
  Graphics::RenderPass RenderPass;
  Graphics::FrameBuffer FrameBuffer;
  Graphics::Renderer Renderer;
  Graphics::Shader VertexShader;
  Graphics::Shader FragmentShader;
  Graphics::Pipeline Pipeline;
  Graphics::CommandPool Pool;
  Graphics::CommandBuffer Buffer;
  Graphics::DescriptorPool DescriptorPool;

  Graphics::Buffer TempBuffer;
  Graphics::Buffer TempBuffer2;

  //Screen
  Graphics::Buffer VertexBuffer;
  Graphics::Buffer IndexBuffer;
  std::vector<glm::vec3> vertices = {
      {-1.0f, -1.0f, 0.0f},
      {-1.0f, 1.0f, 0.0f},
      {1.0f, 1.0f, 0.0f},
      {1.0f, -1.0f, 0.0f}};
  const std::vector<uint16_t> indices = {
      0, 1, 2, 2, 3, 0};

public:
  void OnAwake()
  {
    ApplicationDir = GetExecutablePath();

    Engine = Graphics::CreateRenderingEngine();
    Window = Graphics::CreateWindow(
        Engine,
        "Tortuga",
        1024, 768,
        Graphics::WindowType::ResizeableWindowed);

    Hardware = Graphics::CreateHardwareController(
        Engine,
        Window,
        Engine.Devices);

    RenderPass = Graphics::CreateRenderPass(Hardware);
    FrameBuffer = Graphics::CreateFrameBuffers(Hardware, RenderPass);
    Renderer = Graphics::CreateRenderer(Hardware, Window, FrameBuffer, RenderPass);

    VertexShader = Graphics::CreateShaderFromFile(
        Hardware,
        ApplicationDir + "/Shaders/simple.vert.spv",
        Graphics::SHADER_TYPE_VERTEX);

    FragmentShader = Graphics::CreateShaderFromFile(
        Hardware,
        ApplicationDir + "/Shaders/simple.frag.spv",
        Graphics::SHADER_TYPE_FRAGMENT);
    Pipeline = Graphics::CreatePipeline(Hardware, RenderPass, {VertexShader, FragmentShader});

    Pool = Graphics::CreateCommandPool(Hardware);
    Buffer = Graphics::CreateCommandBuffer(Hardware, Pool, Graphics::COMMAND_BUFFER_SECONDARY, 1);

    VertexBuffer = Graphics::CreateBuffer(
        Hardware,
        Graphics::BUFFER_TYPE_VERTEX,
        sizeof(vertices[0]) * vertices.size(),
        Graphics::BUFFER_STORAGE_FAST);
    Graphics::UpdateBufferData(VertexBuffer, vertices);

    IndexBuffer = Graphics::CreateBuffer(
        Hardware,
        Graphics::BUFFER_TYPE_INDEX,
        sizeof(indices[0]) * indices.size(),
        Graphics::BUFFER_STORAGE_FAST);
    Graphics::UpdateBufferData(IndexBuffer, indices);

    {
      auto tempPool = Graphics::CreateCommandPool(Hardware);
      auto tempBuffer = Graphics::CreateCommandBuffer(Hardware, tempPool, Graphics::COMMAND_BUFFER_PRIMARY, 1);
      Graphics::BeginCommandBuffer(tempBuffer, 0);
      Graphics::CommandBufferUpdateBuffer(tempBuffer, 0, VertexBuffer);
      Graphics::CommandBufferUpdateBuffer(tempBuffer, 0, IndexBuffer);
      Graphics::EndCommandBuffer(tempBuffer, 0);
      Graphics::CommandBufferSubmitCommands(tempBuffer);
      Graphics::DestroyCommandPool(tempPool);
    }

    TempBuffer = Graphics::CreateBuffer(Hardware, Graphics::BUFFER_TYPE_UNIFORM, sizeof(Temp), Graphics::BUFFER_STORAGE_ACCESSIBLE);
    Graphics::UpdateBufferData(TempBuffer, (Temp){{1.0, 0.0, 0.0}});
    DescriptorPool = Graphics::CreateDescriptorPool(Hardware, Graphics::DESCRIPTOR_TYPE_UNIFORM, 2);
    auto DescriptorSets = Graphics::ConfigureDescriptorPool(Hardware, Pipeline.Layout, Graphics::DESCRIPTOR_TYPE_UNIFORM, DescriptorPool);

    Graphics::ConfigureDescriptorSet(DescriptorSets, TempBuffer, 0, 0);
    Graphics::ConfigureDescriptorSet(DescriptorSets, TempBuffer, 0, 1);

    {
      Graphics::BeginCommandBuffer(Buffer, 0, RenderPass, 0);
      Graphics::BindCommandBufferPipeline(Buffer, 0, Pipeline);
      Graphics::CommandBufferBindDescriptor(Buffer, 0, Pipeline, {DescriptorSets});
      Graphics::CommandBufferDraw(Buffer, 0, VertexBuffer, IndexBuffer, indices.size());
      Graphics::EndCommandBuffer(Buffer, 0);
    }

    Graphics::SubmitCommands(Renderer, {Buffer});
  }

  void OnUpdate()
  {
    Graphics::DrawFrame(Renderer);
  }

  void OnEnd()
  {
    Graphics::DestroyRenderer(Renderer);
    Graphics::DestroyDescriptorPool(DescriptorPool);
    Graphics::DestroyBuffer(TempBuffer);
    Graphics::DestroyBuffer(VertexBuffer);
    Graphics::DestroyBuffer(IndexBuffer);
    Graphics::DestroyCommandPool(Pool);
    Graphics::DestroyPipeline(Pipeline);
    Graphics::DestroyShader(VertexShader);
    Graphics::DestroyShader(FragmentShader);
    Graphics::DestroyFrameBuffers(FrameBuffer);
    Graphics::DestroyRenderPass(RenderPass);
    Graphics::DestroyWindow(Window);
    Graphics::DestroyRenderingEngine(Engine);
  }
};
}; // namespace Tortuga

#endif