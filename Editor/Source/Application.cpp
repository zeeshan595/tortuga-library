#include <Tortuga.h>

int main(int argc, char **argv)
{
    Tortuga::InitializeEngine("Application");
    Tortuga::MainLoop();
    Tortuga::DestroyEngine();

    return 0;
}