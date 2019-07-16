#include "./Console.hpp"

void Tortuga::Console::ProcessLog(Tortuga::Console::Level level, std::string message, Tortuga::Console::Arguments arguments)
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

void Tortuga::Console::Print(std::string message, Arguments args)
{
  ProcessLog(LEVEL_NONE, message, args);
}
void Tortuga::Console::Info(std::string message, Tortuga::Console::Arguments args)
{
#if DEBUG_MODE
  ProcessLog(LEVEL_INFO, message, args);
#endif
}
void Tortuga::Console::Debug(std::string message, Tortuga::Console::Arguments args)
{
#if DEBUG_MODE
  ProcessLog(LEVEL_DEBUG, message, args);
#endif
}
void Tortuga::Console::Warning(std::string message, Tortuga::Console::Arguments args)
{
#if DEBUG_MODE
  ProcessLog(LEVEL_WARNING, message, args);
#endif
}
void Tortuga::Console::Error(std::string message, Tortuga::Console::Arguments args)
{
  ProcessLog(LEVEL_ERROR, message, args);
}
void Tortuga::Console::Fatal(std::string message, Tortuga::Console::Arguments args)
{
  ProcessLog(LEVEL_FATAL, message, args);
  std::runtime_error("Console fatal triggered!");
}