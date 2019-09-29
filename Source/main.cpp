#include "./Tortuga.hpp"

using namespace Tortuga;

int main()
{
  const auto temp = Systems::Rendering();
  while (true) {
    temp.Update();
  }
  return EXIT_SUCCESS;
}