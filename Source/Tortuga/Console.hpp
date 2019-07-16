#ifndef _TORTUGA_CONSOLE
#define _TORTUGA_CONSOLE

#include <iostream>
#include <sstream>
#include <ostream>
#include <vector>
#include <glm/glm.hpp>

namespace Tortuga
{
namespace Console
{
const int TEXT_BLACK = 30;
const int TEXT_RED = 31;
const int TEXT_GREEN = 32;
const int TEXT_YELLOW = 33;
const int TEXT_BLUE = 34;
const int TEXT_PURPLE = 35;
const int TEXT_CYAN = 36;
const int TEXT_WHITE = 37;

const int BG_BLACK = 40;
const int BG_RED = 41;
const int BG_GREEN = 42;
const int BG_YELLOW = 43;
const int BG_BLUE = 44;
const int BG_PURPLE = 45;
const int BG_CYAN = 46;
const int BG_WHITE = 47;
class Arguments
{
private:
  std::vector<std::string> _args;

public:
  Arguments()
  {
  }
  Arguments(bool b)
  {
    *this << b;
  }
  Arguments(int i)
  {
    *this << i;
  }
  Arguments(uint32_t i)
  {
    *this << i;
  }
  Arguments(float i)
  {
    *this << i;
  }
  Arguments(std::size_t i)
  {
    *this << i;
  }
  Arguments(long i)
  {
    *this << i;
  }
  Arguments(long long i)
  {
    *this << i;
  }
  Arguments(const char str)
  {
    *this << str;
  }
  Arguments(const char *str)
  {
    *this << str;
  }
  Arguments(std::string str)
  {
    *this << str;
  }
  Arguments(glm::vec2 v)
  {
    *this << v.x << ", " << v.y;
  }

  std::vector<std::string> str()
  {
    return _args;
  }

  Arguments &operator<<(bool b)
  {
    std::ostringstream stream;
    if (b)
      stream << "True";
    else
      stream << "False";

    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(int i)
  {
    std::ostringstream stream;
    stream << i;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(uint32_t i)
  {
    std::ostringstream stream;
    stream << i;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(float i)
  {
    std::ostringstream stream;
    stream << i;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(std::size_t i)
  {
    std::ostringstream stream;
    stream << i;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(long i)
  {
    std::ostringstream stream;
    stream << i;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(long long i)
  {
    std::ostringstream stream;
    stream << i;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(const char c)
  {
    std::ostringstream stream;
    stream << c;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(const char *str)
  {
    std::ostringstream stream;
    stream << str;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(std::string str)
  {
    std::ostringstream stream;
    stream << str;
    _args.push_back(stream.str());
    return *this;
  }
  Arguments &operator<<(glm::vec2 v)
  {
    std::ostringstream stream;
    stream << v.x << ", " << v.y;
    _args.push_back(stream.str());
    return *this;
  }
};
enum Level
{
  LEVEL_NONE,
  LEVEL_INFO,
  LEVEL_DEBUG,
  LEVEL_WARNING,
  LEVEL_ERROR,
  LEVEL_FATAL
};

void ProcessLog(Level level, std::string message, Arguments args);

void Print(std::string message, Arguments args = {});
void Info(std::string message, Arguments args = {});
void Debug(std::string message, Arguments args = {});
void Warning(std::string message, Arguments args = {});
void Error(std::string message, Arguments args = {});
void Fatal(std::string message, Arguments args = {});

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
}
}; // namespace Console
}; // namespace Tortuga

#endif
