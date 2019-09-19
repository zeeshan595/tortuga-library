#ifndef _WAYLAND_BUFFER
#define _WAYLAND_BUFFER

namespace Tortuga
{
namespace Graphics
{
namespace DisplayServer
{
namespace Wayland
{
struct Buffer
{
};
Buffer CreateBuffer();
void DestroyBuffer(Buffer data);
} // namespace Wayland
} // namespace DisplayServer
} // namespace Graphics
} // namespace Tortuga

#endif