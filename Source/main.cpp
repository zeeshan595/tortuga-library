#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  Graphics::Screen::Initialize();
  const auto window = Graphics::Screen::CreateWindow("Tortuga", 1920, 1080);
  //todo: Graphics::Screen::UpdateRenderData(window, {m});

  auto data = new Graphics::Mesh();
  data->BufferData.VerticesSize = 5;
  data->BufferData.VerticesSize = 3;
  auto entity = Core::Entity::Create();
  entity->AddComponent<Graphics::Mesh>(data);
  auto temp = entity->GetComponent<Graphics::Mesh>();

  Core::CreateSystem<Systems::Example>();
  Core::DestroySystem<Systems::Example>();

  entity->RemoveComponent<Graphics::Mesh>();
  Core::Entity::Destroy(entity);

  bool signalClose = false;
  while (!signalClose)
  {
    const auto event = Graphics::Screen::PollEvents(window);
    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
      signalClose = true;
  }

  return 0;
}