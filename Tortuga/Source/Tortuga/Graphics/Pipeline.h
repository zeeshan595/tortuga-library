#ifndef _PIPELINE
#define _PIPELINE

#include "VulkanAPI/DataStructures.h"
#include "VulkanAPI/RenderPass.h"
#include "VulkanAPI/Pipeline.h"

#include "Window.h"
#include "RenderPass.h"
#include "Shader.h"

namespace Tortuga
{
namespace Graphics
{
struct Pipeline
{
  std::vector<VulkanAPI::PipelineData> VulkanPipeline;
};

Pipeline CreatePipeline(Window window, RenderPass renderPass, std::vector<Shader> shaders);
void DestroyPipeline(Pipeline data);

}; // namespace Graphics
}; // namespace Tortuga

#endif