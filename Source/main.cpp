#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  const auto vulkan = Graphics::Vulkan::Instance::Create();
  const auto device = vulkan.Devices[0];

  //Shader
  const auto shaderCode = Utils::IO::GetFileContents("Shaders/ray-marching.comp");
  const auto compiledShaderCode = Graphics::Vulkan::Shader::CompileShader(vulkan, Graphics::Vulkan::Shader::COMPUTE, shaderCode);
  const auto shader = Graphics::Vulkan::Shader::Create(device, compiledShaderCode);

  //Descriptor Layout
  std::vector<Graphics::Vulkan::DescriptorLayout::Binding> bindings(2);
  {
    bindings[0].DescriptorCount = 1;
    bindings[0].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[0].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
    bindings[0].Sampler = VK_NULL_HANDLE;

    bindings[1].DescriptorCount = 1;
    bindings[1].Type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[1].ShaderStage = VK_SHADER_STAGE_COMPUTE_BIT;
    bindings[1].Sampler = VK_NULL_HANDLE;
  }
  const auto descriptorLayout = Graphics::Vulkan::DescriptorLayout::Create(device, bindings);

  //pipeline
  const auto pipeline = Graphics::Vulkan::Pipeline::CreateComputePipeline(device, shader, {}, {descriptorLayout});

  //Descriptor set
  const auto descriptorPool = Graphics::Vulkan::DescriptorPool::Create(device, descriptorLayout);
  const auto descriptorSet = Graphics::Vulkan::DescriptorSets::Create(device, descriptorPool, {descriptorLayout});

  //buffer
  const auto bufferInput = Graphics::Vulkan::Buffer::Create(device, sizeof(uint32_t) * 2, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  const auto bufferOutput = Graphics::Vulkan::Buffer::Create(device, sizeof(uint32_t), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
  Graphics::Vulkan::DescriptorSets::UpdateDescriptorSets(descriptorSet, 0, {bufferInput, bufferOutput});

  //command
  const auto commandPool = Graphics::Vulkan::CommandPool::Create(device, device.QueueFamilies.Compute.Index);
  const auto command = Graphics::Vulkan::Command::Create(device, commandPool, Graphics::Vulkan::Command::PRIMARY);

  //Record comand
  Graphics::Vulkan::Command::Begin(command, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
  Graphics::Vulkan::Command::BindPipeline(command, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline, {descriptorSet});
  Graphics::Vulkan::Command::Compute(command, 1, 1, 1);
  Graphics::Vulkan::Command::End(command);

  //Update input data
  std::vector<uint32_t> inputData = {2, 5};
  Graphics::Vulkan::Buffer::SetData(bufferInput, inputData.data(), sizeof(uint32_t) * inputData.size());

  //Submit command
  Graphics::Vulkan::Command::Submit({command}, device.Queues.Compute[0]);
  //Wait
  Graphics::Vulkan::Device::WaitForQueue(device.Queues.Compute[0]);

  //Get output data
  uint32_t outputData;
  Graphics::Vulkan::Buffer::GetData(bufferOutput, &outputData, sizeof(uint32_t));
  Console::Info("Output: {0}", outputData);

  Graphics::Vulkan::CommandPool::Destroy(commandPool);
  Graphics::Vulkan::Buffer::Destroy(bufferOutput);
  Graphics::Vulkan::Buffer::Destroy(bufferInput);
  Graphics::Vulkan::DescriptorPool::Destroy(descriptorPool);
  Graphics::Vulkan::Pipeline::DestroyPipeline(pipeline);
  Graphics::Vulkan::DescriptorLayout::Destroy(descriptorLayout);
  Graphics::Vulkan::Shader::Destroy(shader);
  Graphics::Vulkan::Instance::Destroy(vulkan);
}