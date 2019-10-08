#ifndef _TORTUGA_CONSOLE
#define _TORTUGA_CONSOLE

#include <iostream>
#include <sstream>
#include <ostream>
#include <vector>

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
  Arguments() {}
  template <typename T>
  Arguments(T element)
  {
    *this << element;
  }
  template <typename T>
  Arguments &operator<<(T element)
  {
    std::ostringstream stream;
    stream << element;
    _args.push_back(stream.str());
    return *this;
  }

  std::vector<std::string> str()
  {
    return _args;
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
}; // namespace Console
}; // namespace Tortuga

#endif
