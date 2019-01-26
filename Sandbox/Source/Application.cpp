#include <Tortuga.h>

class Sandbox : public Tortuga::Application
{
  public:
    Sandbox()
    {
        this->ApplicationName = "Cool Application";
    }
};

Tortuga::Application *Tortuga::CreateApplication()
{
    return new Sandbox();
}