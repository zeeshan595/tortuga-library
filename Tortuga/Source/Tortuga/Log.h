#ifndef _LOGGER
#define _LOGGER

#include <string>
#include <iostream>

namespace Tortuga
{
class Log
{
public:
  static void Debug(std::string message);
  static void Info(std::string message);
  static void Warning(std::string message);
  static void Error(std::string message);
};
}; // namespace Tortuga

#endif