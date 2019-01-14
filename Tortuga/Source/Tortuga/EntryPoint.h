#ifndef _ENTERY_POINT
#define _ENTERY_POINT

extern Tortuga::Application* Tortuga::CreateApplication();

int main(int argc, char** argv)
{
    Tortuga::Log::Init();
    Tortuga::Log::GetEngineLogger()->warn("Initialized!");
    Tortuga::Log::GetApplicationLogger()->info("Application Init");

    auto app = Tortuga::CreateApplication();
    app->Run();
    delete app;
}

#endif