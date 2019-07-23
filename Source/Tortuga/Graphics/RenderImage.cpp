#include "./RenderImage.hpp"

namespace Tortuga
{
namespace Graphics
{
namespace RenderImage
{
RenderImage Create(uint32_t width, uint32_t height)
{
    RenderImage data = {};
    data.Width = width;
    data.Height = height;
    data.Pixels.resize(width * height);
    data.SizeInBytes = sizeof(Pixel) * width * height;
    return data;
}
}; // namespace RenderImage
} // namespace Graphics
} // namespace Tortuga