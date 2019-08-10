#include "./GUID.hpp"

namespace Tortuga
{
namespace Core
{
namespace GUID
{
unsigned char RandomChar()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);
  return static_cast<unsigned char>(dis(gen));
}
std::string GenerateGUID(const unsigned int len)
{
  std::stringstream ss;
  for (uint32_t i = 0; i < len; i++)
  {
    auto rc = RandomChar();
    std::stringstream hexstream;
    hexstream << std::hex << int(rc);
    auto hex = hexstream.str();
    ss << (hex.length() < 2 ? '0' + hex : hex);
  }
  return ss.str();
}
} // namespace GUID
} // namespace Core
} // namespace Tortuga