#ifndef _CONSOLE
#define _CONSOLE

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

  static std::string MakeString(std::string message, std::vector<const char *> args)
  {
    if (args.size() == 0)
      return message;

    for (uint32_t i = 0; i < args.size(); i++)
    {
      std::ostringstream argString;
      argString << "{" << i << "}";
      auto index = message.find(argString.str());
      message.replace(index, index + argString.str().size(), args[i]);
    }

    return message;
  }

public:
  template<typename ...Strings>
  static void Info(std::string message, const Strings&... rest)
  {
#if DEBUG_MODE
    std::cout << "\033[" << BG_BLACK << ";" << TEXT_WHITE << "m" << MakeString(message, { rest... }) << " \033[0m" << std::endl;
#endif
  }
  
  template<typename ...Strings>
  static void Warning(std::string message, const Strings&... rest)
  {
#if DEBUG_MODE
    std::cout << "\033[" << BG_BLACK << ";" << TEXT_YELLOW << "m" << MakeString(message, { rest... }) << " \033[0m" << std::endl;
#endif
  }

  template<typename ...Strings>
  static void Error(std::string message, const Strings&... rest)
  {
    std::cout << "\033[" << BG_BLACK << ";" << TEXT_RED << "m" << MakeString(message, { rest... }) << " \033[0m" << std::endl;
  }

  template<typename ...Strings>
  static void Fatal(std::string message, const Strings&... rest)
  {
    std::cout << "\033[" << BG_BLACK << ";" << TEXT_RED << "m" << MakeString(message, { rest... }) << " \033[0m" << std::endl;
    throw std::runtime_error(message);
  }
};
}; // namespace Tortuga

#endif