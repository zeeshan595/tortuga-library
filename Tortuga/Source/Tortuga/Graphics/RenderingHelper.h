#ifndef _RENDERING_HELPER
#define _RENDERING_HELPER

#include "RenderingEngine.h"
#include "Window.h"
#include "HardwareController.h"
#include "RenderPass.h"
#include "FrameBuffers.h"
#include "Renderer.h"
#include "Pipeline.h"
#include "Shader.h"
#include "Vertex.h"

namespace Tortuga
{
namespace Graphics
{
struct RenderObjects
{
  std::vector<Vertex> Vertices;
  std::vector<uint16_t> Indices;
};
struct RenderObjectBuffers
{
  uint32_t indicesSize;
  Buffer VertexBuffer;
  Buffer IndexBuffer;
  CommandBuffer Command;
};
struct RenderingHelper
{
  HardwareController Hardware;
  RenderPass renderPass;
  FrameBuffer frameBuffer;
  Renderer renderer;
  Pipeline pipeline;
  std::vector<RenderObjects> Objects;
  std::vector<RenderObjectBuffers> ObjectBuffers;
};
RenderingHelper CreateRenderingHelper(HardwareController hardware, Window window, std::vector<Shader> shaders);
void DestroyRenderingHelper(RenderingHelper data);
void SubmitRenderObjects(RenderingHelper data, std::vector<RenderObjects> objects);
void DrawFrame(RenderingHelper data);
}; // namespace Graphics
}; // namespace Tortuga

#endif