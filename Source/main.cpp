#include "./Tortuga.hpp"

using namespace Tortuga;

#define MAX_VERTICES_SIZE 3 //65535
#define MAX_INDICES_SIZE 3
struct Vertex
{
  glm::vec4 Position;
  glm::vec4 Normal;
};
struct Mesh
{
  int32_t VerticesSize;
  int32_t IndicesSize;
  glm::mat4x4 Transformation;
  glm::mat4x4 NormalMatrix;
  Vertex Vertices[MAX_VERTICES_SIZE];
  int32_t Indices[MAX_INDICES_SIZE];
};

int main()
{
  Mesh meshData = {};
  //create mesh data
  {
    meshData.IndicesSize = 3;
    meshData.VerticesSize = 3;
    meshData.Indices[0] = 0;
    meshData.Indices[1] = 1;
    meshData.Indices[2] = 2;
    meshData.Vertices[0] = {
        {0, 1, 0, 1},
        {0, 0, 1, 1}};
    meshData.Vertices[1] = {
        {-1, 0, 0, 1},
        {0, 0, 1, 1}};
    meshData.Vertices[2] = {
        {1, 0, 0, 1},
        {0, 0, 1, 1}};
    meshData.Transformation = glm::mat4(
        glm::vec4(1, 2, 3, 4),
        glm::vec4(5, 6, 7, 8),
        glm::vec4(8, 9, 10, 11),
        glm::vec4(12, 13, 14, 15));
  }

  //Buffer
  auto buffer = Graphics::Vulkan::Buffer::CreateHost(Core::Engine::GetMainDevice(), sizeof(meshData));
  Graphics::Vulkan::Buffer::SetData(buffer, &meshData, sizeof(meshData));

  //descriptor sets
  auto descriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(Core::Engine::GetMainDevice());
  auto descriptorPool = Graphics::Vulkan::DescriptorPool::Create(Core::Engine::GetMainDevice(), {descriptorLayout});
  auto descriptor = Graphics::Vulkan::DescriptorSets::Create(Core::Engine::GetMainDevice(), descriptorPool, descriptorLayout);
  Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(descriptor, {buffer});

  //pipeline
  auto shaderCode = Utils::IO::GetFileContents("Shaders/Geometry.comp");
  auto compiledCode = Graphics::Vulkan::Shader::CompileShader(Core::Engine::GetVulkan(), Graphics::Vulkan::Shader::COMPUTE, shaderCode);
  auto shader = Graphics::Vulkan::Shader::Create(Core::Engine::GetMainDevice(), compiledCode);
  auto pipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(Core::Engine::GetMainDevice(), shader, {}, {descriptorLayout});

  //command buffer
  auto commandPool = Graphics::Vulkan::CommandPool::Create(Core::Engine::GetMainDevice(), Core::Engine::GetMainDevice().QueueFamilies.Compute.Index);
  auto commandBuffer = Graphics::Vulkan::Command::Create(Core::Engine::GetMainDevice(), commandPool, Graphics::Vulkan::Command::PRIMARY);

  //record command buffer
  {
    Graphics::Vulkan::Command::Begin(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::Vulkan::Command::BindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline, {descriptor});
    Graphics::Vulkan::Command::Compute(commandBuffer, 1, 1, 1);
    Graphics::Vulkan::Command::End(commandBuffer);
  }

  //submit recorded command buffer
  Graphics::Vulkan::Command::Submit({commandBuffer}, Core::Engine::GetMainDevice().Queues.Compute[0]);

  //wait for command buffer to finish
  Graphics::Vulkan::Device::WaitForQueue(Core::Engine::GetMainDevice().Queues.Compute[0]);

  //get output from command buffer
  Graphics::Vulkan::Buffer::GetData(buffer, &meshData, sizeof(meshData));

  Console::Info("Done!");
  return 0;
}

/*
int main()
{
  Core::Screen::SetWindowTitle("Hello World");
  Core::Screen::ResizeWindow(1920, 1080);

  //Start rendering system
  Core::CreateSystem<Systems::Rendering>();

  //create entity
  auto entity = Core::Entity::Create();

  Component::Mesh meshData = {};
  meshData.SetVertices({{{0, 1, 0, 1},
                         {0, 0, -1, 1}},
                        {{-1, 0, 0, 1},
                         {0, 0, -1, 1}},
                        {{1, 0, 0, 1},
                         {0, 0, -1, 1}}});
  meshData.SetIndices({0, 1, 2});
  meshData.ApplyTransformation({0, 0, 10}, {0, 0, 0, 1}, {1, 1, 1});

  //attach transform and mesh component
  entity->AddComponent<Component::Transform>();
  entity->AddComponent<Component::Mesh>(&meshData);

  Core::IterateSystemLoop();

  //Main Loop
  bool shouldClose = false;
  while (!shouldClose)
  {
    //Window events
    auto event = Core::Screen::PollEvents();
    //Quit on close pressed
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      shouldClose = true;

    //iterate through all system and execute update functions
    //Core::IterateSystemLoop();
  }

  //remove transform and mesh component
  entity->RemoveComponent<Component::Mesh>();
  entity->RemoveComponent<Component::Transform>();

  //destroy entity
  Core::Entity::Destroy(entity);

  //destroy rendering system
  Core::DestroySystem<Systems::Rendering>();
  return 0;
}
*/