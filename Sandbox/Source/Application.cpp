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

int main()
{
    auto sandbox = new Sandbox();
    sandbox->Run();
    delete sandbox;
    return 0;
}