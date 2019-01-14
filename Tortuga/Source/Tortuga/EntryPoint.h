#ifndef _ENTERY_POINT
#define _ENTERY_POINT

extern Tortuga::Application* Tortuga::CreateApplication();

int main(int argc, char** argv)
{
    auto app = Tortuga::CreateApplication();
    app->Run();
    delete app;
}

#endif