#ifndef _RENDER_TARGET
#define _RENDER_TARGET

#include "./Vulkan/Framebuffer.hpp"
#include "./Vulkan/Image.hpp"
#include "./Vulkan/ImageView.hpp"
#include "./Vulkan/RenderPass.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace CameraRender
{
struct CameraRender
{
  uint32_t Width;
  uint32_t Height;
  Vulkan::Image::Image ColorImage;
  Vulkan::ImageView::ImageView ColorImageView;
  Vulkan::Image::Image DepthImage;
  Vulkan::ImageView::ImageView DepthImageView;
  Vulkan::Framebuffer::Framebuffer Framebuffer;
};
CameraRender Create(Vulkan::Device::Device device, Vulkan::RenderPass::RenderPass renderPass, uint32_t width, uint32_t height);
void Destroy(CameraRender data);
} // namespace RenderTarget
} // namespace Graphics
} // namespace Tortuga

#endif