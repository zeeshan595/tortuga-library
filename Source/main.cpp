#include "./Tortuga.hpp"

using namespace Tortuga;
bool ShouldClose = false;

int main()
{
  //setup engine
  Core::Engine::Create();
  //add a rendering system to the engine
  Core::Engine::AddSystem<Systems::Rendering>();

  //setup events (can use these anywhere)
  Core::Input::NotifyOnWindowClose([] {
    ShouldClose = true;
  });

  const auto camera = Core::Engine::CreateEntity();
  {
    Core::Engine::AddComponent<Components::Camera>(camera);
  }

  //create a cube with a mesh component
  const auto cube = Core::Engine::CreateEntity();
  //const auto mesh = Utils::IO::LoadObjFile("Assets/Models/Cube.obj");
  auto mesh = Components::Mesh();
  mesh.SetVertices({{glm::vec3(0, 0.5, 0)},
                    {glm::vec3(-0.5, 0, 0)},
                    {glm::vec3(0.5, 0, 0)}});
  mesh.SetIndices({0, 1, 2});
  Core::Engine::AddComponent<Components::Mesh>(cube, mesh);

  //main loop
  while (!ShouldClose)
    Core::Engine::IterateSystems();

  //auto destroys everything
  Core::Engine::Destroy();
  return EXIT_SUCCESS;
}