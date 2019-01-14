#include "Log.h"

namespace Tortuga
{
    void Log::Debug(std::string message)
    {
        std::cout << "\033[1;35m" << message << "\033[0m" << std::endl;
    }

    void Log::Info(std::string message)
    {
        std::cout << "\033[1;37m" << message << "\033[0m" << std::endl;
    }

    void Log::Warning(std::string message)
    {
        std::cout << "\033[1;33m" << message << "\033[0m" << std::endl;
    }

    void Log::Error(std::string message)
    {
        std::cout << "\033[1;31m" << message << "\033[0m" << std::endl;
    }
} // namespace Tortuga