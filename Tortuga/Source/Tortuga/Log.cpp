#include "Log.h"

namespace Tortuga
{
    void Log::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        _engineLogger = spdlog::stdout_color_mt("TORTUGA");
        _engineLogger->set_level(spdlog::level::trace);

        _applicationLogger = spdlog::stdout_color_mt("APP");
        _applicationLogger->set_level(spdlog::level::trace);
    }
}