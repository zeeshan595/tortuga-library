#include <Tortuga.h>

using namespace Tortuga;

const std::vector<Graphics::Vertex> vertices = {
    {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0};

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    auto scene = Environment();
    auto plane = CreateEntity(scene, "Plane");
    auto mesh = MeshRenderer();
    mesh.vertices = vertices;
    mesh.indices = indices;
    plane->Data.push_back(mesh);

    auto systemController = CreateSystemController(&scene);
    AddSystem<RenderingSystem>(systemController);

    //Main loop
    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event) != 0)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                isRunning = false;
                break;
            }
        }

        ProcessSystemUpdate(systemController);
    }

    DestroySystemController(systemController);

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}