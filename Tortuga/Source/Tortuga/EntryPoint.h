#ifndef _ENTERY_POINT
#define _ENTERY_POINT

extern Tortuga::Application* Tortuga::CreateApplication();

int main(int argc, char** argv)
{
    auto app = Tortuga::CreateApplication();
    app->Initialize();
    app->Run();
    app->Destroy();
    delete app;
    return 0;
}

#endif