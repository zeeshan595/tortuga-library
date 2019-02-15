#ifndef _CONSOLE
#define _CONSOLE

#include "Core.h"

namespace Tortuga
{
namespace Console
{
class Logger
{
private:
  std::ostringstream _message;

public:
  Logger(bool b)
  {
    if (b)
      _message << "True";
    else
      _message << "False";
  }
  Logger(int i)
  {
    _message << i;
  }
  Logger(uint32_t i)
  {
    _message << i;
  }
  Logger(const char *str)
  {
    _message << str;
  }
  Logger(char str[])
  {
    _message << str;
  }
  Logger(std::string str)
  {
    _message << str;
  }

  std::string str()
  {
    return _message.str();
  }
  const char *c_str()
  {
    return _message.str().c_str();
  }
};

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

std::string MakeString(std::string message, std::vector<Logger> args)
{
  if (args.size() == 0)
    return message;

  for (uint32_t i = 0; i < args.size(); i++)
  {
    std::ostringstream argString;
    argString << "{" << i << "}";
    auto index = message.find(argString.str());
    message.replace(index, index + argString.str().size(), args[i].str());
  }

  return message;
}

void Info(std::string message, std::vector<Logger> args = {})
{
#if DEBUG_MODE
  std::cout << "\033[" << BG_BLACK << ";" << TEXT_WHITE << "m" << MakeString(message, args) << " \033[0m" << std::endl;
#endif
}

void Warning(std::string message, std::vector<Logger> rest = {})
{
#if DEBUG_MODE
  std::cout << "\033[" << BG_BLACK << ";" << TEXT_YELLOW << "m" << MakeString(message, rest) << " \033[0m" << std::endl;
#endif
}

void Error(std::string message, std::vector<Logger> rest = {})
{
  std::cout << "\033[" << BG_BLACK << ";" << TEXT_RED << "m" << MakeString(message, rest) << " \033[0m" << std::endl;
}

void Fatal(std::string message, std::vector<Logger> rest = {})
{
  std::cout << "\033[" << BG_BLACK << ";" << TEXT_RED << "m" << MakeString(message, rest) << " \033[0m" << std::endl;
  throw std::runtime_error(message);
}
}; // namespace Console
}; // namespace Tortuga

#endif