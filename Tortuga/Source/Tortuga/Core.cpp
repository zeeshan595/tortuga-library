#include "Core.h"

std::string GetExecutablePath()
{
  char currentPath[FILENAME_MAX];
  if (!GetCurrentDir(currentPath, sizeof(currentPath)))
  {
    std::runtime_error("Failed to get application path!");
    return std::string();
  }

  return std::string(currentPath);
}