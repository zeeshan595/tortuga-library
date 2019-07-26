#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  const auto window = Graphics::Screen::CreateWindow("Tortuga", 1920, 1080);
  Graphics::Screen::UpdateRenderData(window, 2);

  bool signalClose = false;
  while (!signalClose)
  {
    const auto event = Graphics::Screen::PollEvents(window);
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      signalClose = true;
  }

  return 0;
}