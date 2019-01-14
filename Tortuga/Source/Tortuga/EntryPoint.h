#ifndef _ENTERY_POINT
#define _ENTERY_POINT

extern Tortuga::Application* Tortuga::CreateApplication();

int main(int argc, char** argv)
{
    Tortuga::Log::Debug("Some Message");
    Tortuga::Log::Info("Info");
    Tortuga::Log::Warning("Warning");
    Tortuga::Log::Error("Error");
    
    auto app = Tortuga::CreateApplication();
    app->Run();
    delete app;
}

#endif