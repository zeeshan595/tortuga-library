#ifndef _GUID
#define _GUID

#include <vector>
#include <iostream>
#include <sstream>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <string>

namespace Tortuga
{
namespace Core
{
namespace GUID
{
std::string GenerateGUID(const unsigned int len = 256);
} // namespace GUID
} // namespace Core
} // namespace Tortuga

#endif