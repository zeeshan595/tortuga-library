#include "Application.h"
#include <iostream>

namespace Tortuga
{
    Application::Application()
    {

    }

    Application::~Application()
    {

    }

    void Application::Run()
    {
        std::cout << "Tortuga Engine Started!" << std::endl;
        while (true);
    }
}