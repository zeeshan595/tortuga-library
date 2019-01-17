#ifndef _CONSOLE
#define _CONSOLE

#include <string>
#include <iostream>
#include <exception>

#include "Core.h"

namespace Tortuga
{
class Console
{
private:
  static const int TEXT_BLACK = 30;
  static const int TEXT_RED = 31;
  static const int TEXT_GREEN = 32;
  static const int TEXT_YELLOW = 33;
  static const int TEXT_BLUE = 34;
  static const int TEXT_PURPLE = 35;
  static const int TEXT_CYAN = 36;
  static const int TEXT_WHITE = 37;

  static const int BG_BLACK = 40;
  static const int BG_RED = 41;
  static const int BG_GREEN = 42;
  static const int BG_YELLOW = 43;
  static const int BG_BLUE = 44;
  static const int BG_PURPLE = 45;
  static const int BG_CYAN = 46;
  static const int BG_WHITE = 47;

public:
  static void Info(std::string message)
  {
#if DEBUG_MODE
    std::cout << "\033[" << BG_BLACK << ";" << TEXT_WHITE << "m" << message << " \033[0m" << std::endl;
#endif
  }
  static void Warning(std::string message)
  {
#if DEBUG_MODE
    std::cout << "\033[" << BG_BLACK << ";" << TEXT_YELLOW << "m" << message << " \033[0m" << std::endl;
#endif
  }
  static void Error(std::string message)
  {
    std::cout << "\033[" << BG_BLACK << ";" << TEXT_RED << "m" << message << " \033[0m" << std::endl;
  }
  static void Fatal(std::string message)
  {
    std::cout << "\033[" << BG_BLACK << ";" << TEXT_RED << "m" << message << " \033[0m" << std::endl;
    throw std::runtime_error(message);
  }
};
}; // namespace Tortuga

#endif