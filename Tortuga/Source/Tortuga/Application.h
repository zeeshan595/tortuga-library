#ifndef _APPLICATION
#define _APPLICATION

#include "Core.h"
#include "Console.h"

namespace Tortuga
{
void InitializeEngine(const char *applicationName);
void MainLoop();
void DestroyEngine();
}; // namespace Tortuga

#endif