#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  const auto window = Graphics::Screen::CreateWindow("Tortuga", 1920, 1080);
  Graphics::Mesh mesh = {};
  std::vector<Graphics::Vertex> vertices(8);
  {
    vertices[0].Position = {-1, -1, -1};
    vertices[1].Position = { 1, -1, -1};
    vertices[2].Position = { 1,  1, -1};
    vertices[3].Position = {-1,  1, -1};
    vertices[4].Position = {-1, -1,  1};
    vertices[5].Position = { 1, -1,  1};
    vertices[6].Position = { 1,  1,  1};
    vertices[7].Position = {-1,  1,  1};
  }
  mesh.SetVertices(vertices);
  mesh.UpdateBounds();
  Graphics::Screen::UpdateRenderData(window, {mesh});

  bool signalClose = false;
  while (!signalClose)
  {
    const auto event = Graphics::Screen::PollEvents(window);
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      signalClose = true;
  }

  return 0;
}