#ifndef _RENDER_IMAGE
#define _RENDER_IMAGE

#include <vulkan/vulkan.h>
#include <vector>

namespace Tortuga
{
namespace Graphics
{
namespace RenderImage
{
struct Pixel
{
    float r;
    float g;
    float b;
};
struct RenderImage
{
    uint32_t Width;
    uint32_t Height;
    std::vector<Pixel> Pixels;
    uint32_t SizeInBytes;
};

RenderImage Create(uint32_t width, uint32_t height);
void SetData(std::vector<Pixel> pixels);
std::vector<Pixel> GetData();
}; // namespace RenderImage
} // namespace Graphics
} // namespace Tortuga

#endif