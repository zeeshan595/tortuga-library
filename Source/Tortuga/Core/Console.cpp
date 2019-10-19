#include "./Console.hpp"

namespace Tortuga
{
namespace Console
{
void ProcessLog(Level level, std::string message, Arguments arguments)
{
  auto args = arguments.str();
  std::ostringstream stream;
  stream << "\033[";
  switch (level)
  {
  default:
  case LEVEL_NONE:
    stream << BG_BLACK << ";" << TEXT_GREEN << "m";
    break;
  case LEVEL_INFO:
    stream << BG_BLACK << ";" << TEXT_WHITE << "m";
    break;
  case LEVEL_DEBUG:
    stream << BG_BLACK << ";" << TEXT_CYAN << "m";
    break;
  case LEVEL_WARNING:
    stream << BG_BLACK << ";" << TEXT_YELLOW << "m";
    break;
  case LEVEL_ERROR:
  case LEVEL_FATAL:
    stream << BG_BLACK << ";" << TEXT_RED << "m";
    break;
  }
  for (uint32_t i = 0; i < args.size(); i++)
  {
    std::ostringstream argString;
    argString << "{" << i << "}";
    auto index = message.find(argString.str());
    message.replace(message.begin() + index, message.begin() + index + argString.str().size(), args[i].c_str());
  }
  stream << message << " \033[0m" << std::endl;
  std::cout << stream.str();
}

void Print(std::string message, Arguments args)
{
  ProcessLog(LEVEL_NONE, message, args);
}
void Info(std::string message, Arguments args)
{
#if DEBUG_MODE
  ProcessLog(LEVEL_INFO, message, args);
#endif
}
void Debug(std::string message, Arguments args)
{
#if DEBUG_MODE
  ProcessLog(LEVEL_DEBUG, message, args);
#endif
}
void Warning(std::string message, Arguments args)
{
#if DEBUG_MODE
  ProcessLog(LEVEL_WARNING, message, args);
#endif
}
void Error(std::string message, Arguments args)
{
  ProcessLog(LEVEL_ERROR, message, args);
}
void Fatal(std::string message, Arguments args)
{
  ProcessLog(LEVEL_FATAL, message, args);
  std::runtime_error("Console fatal triggered!");
  exit(-1);
}
}; // namespace Console
}; // namespace Tortuga