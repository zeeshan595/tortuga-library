#ifndef _APPLICATION
#define _APPLICATION

namespace Tortuga
{
class Application
{
  public:
    Application();
    virtual ~Application();

    void Run();
};

//Will be defined in application
Application *CreateApplication();
}; // namespace Tortuga

#endif