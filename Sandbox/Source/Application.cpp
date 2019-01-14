#include <Tortuga.h>

class Sandbox : public Tortuga::Application
{
  public:
    Sandbox()
    {
    }

    ~Sandbox()
    {
    }
};

Tortuga::Application* Tortuga::CreateApplication()
{
    return new Sandbox();
}