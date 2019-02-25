#include "RenderingHelper.h"

namespace Tortuga
{
namespace Graphics
{
RenderingHelper CreateRenderingHelper(HardwareController hardware, Window window, std::vector<Shader> shaders)
{
  auto data = RenderingHelper();
  data.Hardware = hardware;

  data.renderPass = Graphics::CreateRenderPass(data.Hardware);
  data.frameBuffer = Graphics::CreateFrameBuffers(data.Hardware, data.renderPass);
  data.renderer = Graphics::CreateRenderer(data.Hardware, window, data.frameBuffer, data.renderPass);
  data.pipeline = Graphics::CreatePipeline(data.Hardware, data.renderPass, shaders);

  return data;
}
void DestroyRenderingHelper(RenderingHelper data)
{
  Graphics::DestroyPipeline(data.pipeline);
  Graphics::DestroyFrameBuffers(data.frameBuffer);
  Graphics::DestroyRenderPass(data.renderPass);
}
void SubmitRenderObjects(RenderingHelper data, std::vector<RenderObjects> objects)
{
  uint32_t threadNum = 0;
  for (uint32_t i = 0; i < objects.size(); i++)
  {
  }
}
}; // namespace Graphics
}; // namespace Tortuga
