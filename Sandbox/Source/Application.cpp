#include <Tortuga.h>

class Sandbox : public Tortuga::Application
{
  public:
    Sandbox()
    {
        this->ApplicationName = "Tortuga Testing Tool";
    }
};

Tortuga::Application *Tortuga::CreateApplication()
{
    return new Sandbox();
}