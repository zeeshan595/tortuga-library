#ifndef _MULTI_GPU_RENDERING_SYSTEM
#define _MULTI_GPU_RENDERING_SYSTEM

#include <glm/glm.hpp>
#include <thread>
#include <future>
#include <chrono>
#include <iostream>

#include "../Components/Mesh.h"
#include "../Console.h"
#include "../Core/Engine.h"
#include "../Core/System.h"
#include "../Graphics/VulkanBuffer.h"
#include "../Graphics/VulkanCommand.h"
#include "../Graphics/VulkanDevice.h"
#include "../Graphics/VulkanImage.h"
#include "../Graphics/VulkanPipeline.h"
#include "../Graphics/VulkanShader.h"
#include "../Graphics/VulkanSwapchain.h"
#include "../Utils/InputOutput.h"

namespace Tortuga {
namespace Systems {
struct MultiGPURenderingSystem : public Core::System {
private:
  struct RenderSettings {
    uint32_t WindowWidth;
    uint32_t WindowHeight;
  };
  RenderSettings _settings;
  bool _stopped = false;
  uint32_t _mainDevice = 0;
  // per gpu
  std::vector<Graphics::VulkanDevice> _devices;
  std::vector<Graphics::VulkanShader> _shaders;
  std::vector<Graphics::VulkanPipeline> _pipelines;
  std::vector<Graphics::VulkanBuffer> _renderOffsets;
  std::vector<Graphics::VulkanBuffer> _inputBuffers;
  std::vector<Graphics::VulkanBuffer> _outputBuffers;
  std::vector<Graphics::VulkanCommandPool> _commandPools;
  std::vector<Graphics::VulkanCommand> _renderCommands;
  std::vector<Graphics::VulkanImage> _renderImages;
  std::vector<glm::vec2> _renderImageOffsets; // x = offset x, y = width x

  // main gpu
  std::vector<Graphics::VulkanBuffer> _mainDeviceRenderBuffers;
  std::vector<Graphics::VulkanImage> _mainDeviceRenderImages;
  std::vector<Graphics::VulkanCommand> _transferToMainDevice;
  Graphics::VulkanCommand _copyToSwapchain;
  Graphics::VulkanFence _fence;
  uint32_t _totalOutputBufferSize;

  // window
  Graphics::VulkanSwapchain _swapchain;

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

  void Start() {
    _devices = Core::Vulkan.Devices;

    RegisterComponent<Components::Mesh>();
    _devices.resize(_devices.size());
    _shaders.resize(_devices.size());
    _pipelines.resize(_devices.size());
    _renderOffsets.resize(_devices.size());
    _inputBuffers.resize(_devices.size());
    _outputBuffers.resize(_devices.size());
    _commandPools.resize(_devices.size());
    _renderCommands.resize(_devices.size());
    _renderImages.resize(_devices.size());
    _renderImageOffsets.resize(_devices.size());
    _mainDeviceRenderBuffers.resize(_devices.size());
    _mainDeviceRenderImages.resize(_devices.size());
    _transferToMainDevice.resize(_devices.size());

    // todo: change vector sizes
    for (uint32_t i = 0; i < _devices.size(); i++) {
      // shader computation
      _shaders[i] = Graphics::CreateVulkanShader(
          _devices[i], Utils::GetFileContents("Shaders/compute.spv"));

      // compute pipeline
      _pipelines[i] = Graphics::CreateVulkanPipeline(
          _devices[i], _shaders[i].ShaderModule, 2);

      // buffers for shader
      _renderOffsets[i] =
          Graphics::CreateVulkanBuffer(_devices[i], sizeof(RenderOffsets),
                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
      _inputBuffers[i] =
          Graphics::CreateVulkanBuffer(_devices[i], sizeof(InputBuffer),
                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      _commandPools[i] = Graphics::CreateVulkanCommandPool(_devices[i]);
      _transferToMainDevice[i] =
          Graphics::CreateVulkanCommand(_commandPools[_mainDevice]);
    }
    _copyToSwapchain =
        Graphics::CreateVulkanCommand(_commandPools[_mainDevice]);
    _fence = Graphics::CreateVulkanFence(_devices[_mainDevice]);
    ResizeScreen(1920, 1080);
    Console::Info("Rendering System Started");
  }

  void End() {
    if (_stopped)
      return;

    Graphics::DestroyVulkanFence(_fence);
    for (uint32_t i = 0; i < _devices.size(); i++) {
      if (_outputBuffers[i].Buffer != VK_NULL_HANDLE) {
        Graphics::DestroyVulkanBuffer(_outputBuffers[i]);
      }
      Graphics::DestroyVulkanImage(_renderImages[i]);
      Graphics::DestroyVulkanCommandPool(_commandPools[i]);
      Graphics::DestroyVulkanBuffer(_inputBuffers[i]);
      Graphics::DestroyVulkanBuffer(_outputBuffers[i]);
      Graphics::DestroyVulkanShader(_shaders[i]);
      Graphics::DestroyVulkanPipeline(_pipelines[i]);
    }
    if (Core::Window.SDLWindow != nullptr) {
      Graphics::DestroyVulkanWindow(Core::Window);
      Graphics::DestroyVulkanSwapchain(_swapchain);
    }
    Console::Info("Rendering System Stopped");
  }

  void ResizeScreen(uint32_t width, uint32_t height) {
    _stopped = true;
    Graphics::DeviceQueueWaitForIdle(_devices[_mainDevice],
                                     Graphics::VULKAN_QUEUE_TYPE_COMPUTE);
    Graphics::DeviceQueueWaitForIdle(_devices[_mainDevice],
                                     Graphics::VULKAN_QUEUE_TYPE_PRESENT);
    _settings.WindowWidth = width;
    _settings.WindowHeight = height;
    _totalOutputBufferSize =
        _settings.WindowWidth * _settings.WindowHeight * sizeof(Pixel);
    uint32_t totalScore = 0;
    for (uint32_t i = 0; i < _devices.size(); i++) {
      totalScore += _devices[i].Score;
    }
    uint32_t offsetTracker = 0;
    for (uint32_t i = 0; i < _devices.size(); i++) {
      // image offset and size's using device score
      _renderImageOffsets[i].x =
          (offsetTracker / totalScore) * _settings.WindowWidth;
      _renderImageOffsets[i].y =
          (_devices[i].Score / totalScore) * _settings.WindowWidth;
      offsetTracker += _devices[i].Score;

      // update render image offsets for each gpu
      RenderOffsets imageOffsetData = {_renderImageOffsets[i].x, 0};
      Graphics::SetVulkanBufferData<RenderOffsets>(
          _renderOffsets[i], &imageOffsetData, sizeof(RenderOffsets));

      // set render image sizes for each gpu
      if (_outputBuffers[i].Buffer != VK_NULL_HANDLE) {
        Graphics::DestroyVulkanBuffer(_outputBuffers[i]);
      }

      _outputBuffers[i] =
          Graphics::CreateVulkanBuffer(_devices[i], _totalOutputBufferSize,
                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      // update pipeline with new buffers
      Graphics::UpdatePipelineDescriptors(
          _pipelines[i], {_renderOffsets[i], _outputBuffers[i]});

      // setup render command for each gpu
      _renderCommands[i] = Graphics::CreateVulkanCommand(_commandPools[i]);
      Graphics::VulkanCommandBegin(
          _renderCommands[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
      Graphics::VulkanCommandBindPipeline(_renderCommands[i], _pipelines[i]);
      Graphics::VulkanCommandDispatch(_renderCommands[i],
                                      _renderImageOffsets[i].y,
                                      _settings.WindowHeight, 1);
      Graphics::VulkanCommandEnd(_renderCommands[i]);

      _mainDeviceRenderImages[i] = Graphics::CreateVulkanImage(
          _devices[_mainDevice], _renderImageOffsets[i].y,
          _settings.WindowHeight);
      _mainDeviceRenderBuffers[i] = Graphics::CreateVulkanBuffer(
          _devices[_mainDevice], _totalOutputBufferSize,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      // convert buffers to image on main gpu
      Graphics::VulkanCommandBegin(
          _transferToMainDevice[i],
          VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
      Graphics::VulkanCommandImageLayoutTransfer(
          _transferToMainDevice[i], _mainDeviceRenderImages[i].Image,
          VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      Graphics::VulkanCommandCopyBufferToImage(
          _transferToMainDevice[i], _mainDeviceRenderBuffers[i],
          _mainDeviceRenderImages[i].Image, {0, 0},
          {_renderImageOffsets[i].y, _settings.WindowHeight});
      Graphics::VulkanCommandImageLayoutTransfer(
          _transferToMainDevice[i], _mainDeviceRenderImages[i].Image,
          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
      Graphics::VulkanCommandEnd(_transferToMainDevice[i]);
      _stopped = false;
    }

    // Window
    if (Core::Window.SDLWindow != nullptr) {
      Graphics::DestroyVulkanWindow(Core::Window);
      Graphics::DestroyVulkanSwapchain(_swapchain);
    }
    Core::Window = Graphics::CreateVulkanWindow(
        "Tortuga", _settings.WindowWidth, _settings.WindowHeight);
    Graphics::CreateVulkanSurface(Core::Window, Core::Vulkan.Instance);
    _swapchain =
        Graphics::CreateVulkanSwapchain(_devices[_mainDevice], Core::Window);
  }

  void Render(uint32_t gpuIndex) {
      // submit render commands on all gpus
      Graphics::VulkanCommandSubmit({_renderCommands[gpuIndex]},
                                    Graphics::VULKAN_QUEUE_TYPE_COMPUTE);
      // Wait for render to finish on all gpus
      Graphics::DeviceQueueWaitForIdle(_devices[gpuIndex],
                                       Graphics::VULKAN_QUEUE_TYPE_COMPUTE);

      // Copy output buffer to main device
      std::vector<Pixel> pixels(_renderImageOffsets[gpuIndex].y *
                                _settings.WindowHeight);
      Graphics::GetVulkanBufferData(_outputBuffers[gpuIndex], pixels.data(),
                                    _totalOutputBufferSize);
      Graphics::SetVulkanBufferData(_mainDeviceRenderBuffers[gpuIndex], pixels.data(),
                                    _totalOutputBufferSize);

      Graphics::VulkanCommandSubmit({_transferToMainDevice[gpuIndex]},
                                    Graphics::VULKAN_QUEUE_TYPE_COMPUTE);
      // Wait for render to finish on all gpus
      Graphics::DeviceQueueWaitForIdle(_devices[_mainDevice],
                                       Graphics::VULKAN_QUEUE_TYPE_COMPUTE);
  }

  void Present() {
    // Get swapchain images
    uint32_t swapchainImageIndex = 0;
    Graphics::FenceReset({_fence});
    Graphics::SwapchainAquireImage(_swapchain, &swapchainImageIndex,
                                   VK_NULL_HANDLE, _fence);
    Graphics::FenceWait({_fence});

    // Present command to copy image to swapchain
    auto copyToPresentImage = CreateVulkanCommand(_commandPools[_mainDevice]);
    Graphics::VulkanCommandBegin(copyToPresentImage,
                                 VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    Graphics::VulkanCommandImageLayoutTransfer(
        copyToPresentImage, _swapchain.Images[swapchainImageIndex],
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    for (uint32_t i = 0; i < _devices.size(); i++) {
      Graphics::VulkanCommandBlitImage(
          copyToPresentImage, _mainDeviceRenderImages[i].Image,
          VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
          _swapchain.Images[swapchainImageIndex],
          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, {_renderImageOffsets[i].x, 0},
          {_renderImageOffsets[i].y, _settings.WindowHeight});
    }
    Graphics::VulkanCommandImageLayoutTransfer(
        copyToPresentImage, _swapchain.Images[swapchainImageIndex],
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    Graphics::VulkanCommandEnd(copyToPresentImage);

    // Submit present command
    Graphics::VulkanCommandSubmit({copyToPresentImage},
                                  Graphics::VULKAN_QUEUE_TYPE_PRESENT);

    // Wait for queue to be idle
    Graphics::DeviceQueueWaitForIdle(_devices[_mainDevice],
                                     Graphics::VULKAN_QUEUE_TYPE_PRESENT);

    Graphics::SwapchainPresentImage(_swapchain, swapchainImageIndex);
  }

  void Update() {
    if (_stopped)
      return;

    //todo: multi-threaded rendering
  }
};
} // namespace Systems
} // namespace Tortuga

#endif
