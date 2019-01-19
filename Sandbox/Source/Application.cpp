#include <Tortuga.h>

class Sandbox : public Tortuga::Application
{
  public:
    Sandbox()
    {
        this->ApplicationName = "Cool Application";
    }

    ~Sandbox()
    {
    }
};

Tortuga::Application *Tortuga::CreateApplication()
{
    return new Sandbox();
}