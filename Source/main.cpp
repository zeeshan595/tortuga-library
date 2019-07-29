#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Graphics::Screen::Initialize();
  const auto window = Graphics::Screen::CreateWindow("Tortuga", 1920, 1080);
  //todo: Graphics::Screen::UpdateRenderData(window, {m});

  auto entity = Core::Entity::Create();
  entity->AddComponent<Graphics::Mesh>();
  auto temp = entity->GetComponent<Graphics::Mesh>();
  entity->RemoveComponent<Graphics::Mesh>();

  bool signalClose = false;
  while (!signalClose)
  {
    const auto event = Graphics::Screen::PollEvents(window);
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      signalClose = true;
  }

  return 0;
}