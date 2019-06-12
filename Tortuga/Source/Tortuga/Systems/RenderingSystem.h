#ifndef _RENDERING_SYSTEM
#define _RENDERING_SYSTEM

#include "../Console.h"
#include "../Core/Engine.h"
#include "../Core/System.h"
#include "../Graphics/VulkanBuffer.h"
#include "../Graphics/VulkanCommand.h"
#include "../Graphics/VulkanDevice.h"
#include "../Graphics/VulkanImage.h"
#include "../Graphics/VulkanPipeline.h"
#include "../Graphics/VulkanShader.h"
#include "../Utils/InputOutput.h"

namespace Tortuga {
namespace Systems {
struct RenderingSystem : public Core::System {
private:
  bool _stopped = false;
  uint32_t _mainDevice = 0;
  std::vector<Graphics::VulkanDevice> _devices;
  std::vector<Graphics::VulkanShader> _shaders;
  std::vector<Graphics::VulkanPipeline> _pipelines;
  std::vector<Graphics::VulkanBuffer> _renderOffsets;
  std::vector<Graphics::VulkanBuffer> _inputBuffers;
  std::vector<Graphics::VulkanBuffer> _outputBuffers;
  std::vector<Graphics::VulkanCommandPool> _commandPools;
  std::vector<Graphics::VulkanCommand> _renderCommands;
  std::vector<Graphics::VulkanImage> _renderImages;
  Graphics::VulkanCommand _combineCommand;
  Graphics::VulkanImage _presentImage;

public:
  struct InputBuffer {
    uint Debug;
  };
  struct RenderOffsets {
    uint32_t OffsetX;
    uint32_t OffsetY;
  };
  struct Pixel {
    float r;
    float g;
    float b;
    float a;
  };
  struct OutputBuffer {
    std::vector<Pixel> Pixels;
  };
  struct RenderSettings {
    uint32_t WindowWidth = 800;
    uint32_t WindowHeight = 600;
  };
  RenderSettings Settings;

  void Start() {
    _devices = Core::Vulkan.Devices;
    _shaders.resize(_devices.size());
    _pipelines.resize(_devices.size());
    _renderOffsets.resize(_devices.size());
    _inputBuffers.resize(_devices.size());
    _outputBuffers.resize(_devices.size());
    _commandPools.resize(_devices.size());
    _renderCommands.resize(_devices.size());
    uint32_t totalScore = 1;
    for (uint32_t i = 0; i < _devices.size(); i++) {
      totalScore += _devices[i].Score;
    }
    for (uint32_t i = 0; i < _devices.size(); i++) {
      _shaders[i] = Graphics::CreateVulkanShader(
          _devices[i], Utils::GetFileContents("Shaders/compute.spv"));

      _pipelines[i] = Graphics::CreateVulkanPipeline(
          _devices[i], _shaders[i].ShaderModule, 3);

      _renderOffsets[i] =
          Graphics::CreateVulkanBuffer(_devices[i], sizeof(RenderOffsets),
                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      _inputBuffers[i] =
          Graphics::CreateVulkanBuffer(_devices[i], sizeof(InputBuffer),
                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      _commandPools[i] = Graphics::CreateVulkanCommandPool(_devices[i]);
      _renderCommands[i] = Graphics::CreateVulkanCommand(_commandPools[i]);

      // Setup Rendering
      Graphics::SetVulkanBufferData<RenderOffsets>(
          _renderOffsets[i], {(_devices[i].Score / totalScore), 0});
    }
    Resize();
    Console::Info("Rendering System Started");
  }

  void End() {
    if (_stopped)
      return;

    for (uint32_t i = 0; i < _devices.size(); i++) {
      Graphics::DestroyVulkanImage(_renderImages[i]);
      Graphics::DestroyVulkanCommandPool(_commandPools[i]);
      Graphics::DestroyVulkanBuffer(_inputBuffers[i]);
      Graphics::DestroyVulkanBuffer(_outputBuffers[i]);
      Graphics::DestroyVulkanShader(_shaders[i]);
      Graphics::DestroyVulkanPipeline(_pipelines[i]);
    }
    Console::Info("Rendering System Stopped");
  }

  void Resize() {
    for (uint32_t i = 0; i < _devices.size(); i++) {
      if (_outputBuffers[i].Buffer != VK_NULL_HANDLE) {
        Graphics::DestroyVulkanBuffer(_outputBuffers[i]);
      }
      _outputBuffers[i] = Graphics::CreateVulkanBuffer(
          _devices[i], sizeof(Settings.WindowWidth * Settings.WindowHeight),
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
      Graphics::UpdatePipelineDescriptors(
          _pipelines[i],
          {_renderOffsets[i], _inputBuffers[i], _outputBuffers[i]});

      Graphics::VulkanCommandBegin(
          _renderCommands[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
      Graphics::VulkanCommandBindPipeline(_renderCommands[i], _pipelines[i]);
      Graphics::VulkanCommandDispatch(_renderCommands[i], Settings.WindowWidth,
                                      Settings.WindowHeight, 1);
      Graphics::VulkanCommandEnd(_renderCommands[i]);
    }

    if (_presentImage.Image != VK_NULL_HANDLE) {
      Graphics::DestroyVulkanImage(_presentImage);
    }
    _presentImage = Graphics::CreateVulkanImage(
        _devices[_mainDevice], Settings.WindowWidth, Settings.WindowHeight);

    if (_combineCommand.CommandBuffer == VK_NULL_HANDLE) {
      _combineCommand =
          Graphics::CreateVulkanCommand(_commandPools[_mainDevice]);
    }
    Graphics::VulkanCommandBegin(_combineCommand, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
    
    Graphics::VulkanCommandEnd(_combineCommand);
  }

  void Update() {
    if (_stopped)
      return;

    for (uint32_t i = 0; i < _devices.size(); i++) {
      Graphics::VulkanCommandSubmit({_renderCommands[i]},
                                    Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

      Graphics::DeviceQueueWaitForIdle(_devices[i],
                                       Graphics::VULKAN_QUEUE_TYPE_COMPUTE);
    }
  }
};
} // namespace Systems
} // namespace Tortuga

#endif