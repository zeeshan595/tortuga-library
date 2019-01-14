#ifndef _LOG
#define _LOG

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace Tortuga
{
class Log
{
  private:
    static std::shared_ptr<spdlog::logger> _engineLogger;
    static std::shared_ptr<spdlog::logger> _applicationLogger;

  public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return _engineLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetApplicationLogger() { return _applicationLogger; }
};
}; // namespace Tortuga

#endif