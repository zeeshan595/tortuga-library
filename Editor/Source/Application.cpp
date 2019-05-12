#include <Tortuga.h>

using namespace Tortuga;

struct Temporary
{
    std::string message;
};

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Console::Info("Tortuga Engine Started!");

    

    Console::Info("Shutting Down!");
    SDL_Quit();

    return 0;
}