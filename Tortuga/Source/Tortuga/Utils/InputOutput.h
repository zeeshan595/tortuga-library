#ifndef _UTILS_INPUT_OUTPUT
#define _UTILS_INPUT_OUTPUT

#include "../Console.h"

#include <vector>
#include <string>
#include <fstream>

namespace Tortuga {
  namespace Utils {
    std::vector<char> GetFileContents(std::string filePath);
  }
}

#endif