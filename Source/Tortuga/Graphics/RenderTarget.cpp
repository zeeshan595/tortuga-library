#include "./RenderTarget.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace RenderTarget
{
RenderTarget Create(Vulkan::Device::Device device, Vulkan::RenderPass::RenderPass renderPass, uint32_t width, uint32_t height)
{
  auto data = RenderTarget();
  data.Width = width;
  data.Height = height;
  data.ColorImage = Vulkan::Image::Create(device, width, height, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
  data.ColorImageView = Vulkan::ImageView::Create(device, data.ColorImage, VK_IMAGE_ASPECT_COLOR_BIT);
  data.DepthImage = Vulkan::Image::Create(device, width, height, VK_FORMAT_D32_SFLOAT, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT);
  data.DepthImageView = Vulkan::ImageView::Create(device, data.DepthImage, VK_IMAGE_ASPECT_DEPTH_BIT);
  data.Framebuffer = Vulkan::Framebuffer::Create(device, width, height, renderPass, {data.ColorImageView, data.DepthImageView});

  return data;
}
void Destroy(RenderTarget data)
{
  Vulkan::Framebuffer::Destroy(data.Framebuffer);
  Vulkan::Image::Destroy(data.ColorImage);
  Vulkan::ImageView::Destroy(data.ColorImageView);
  Vulkan::Image::Destroy(data.ColorImage);
  Vulkan::ImageView::Destroy(data.DepthImageView);
}
} // namespace RenderTarget
} // namespace Graphics
} // namespace Tortuga