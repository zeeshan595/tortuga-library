#ifndef _EXAMPLE_SYSTEM
#define _EXAMPLE_SYSTEM

#include "../Core/System.hpp"
#include "../Graphics/Mesh.hpp"

namespace Tortuga
{
namespace Systems
{
class Example : public Core::System
{
public:
  void Start()
  {
    auto data = this->GetData<Graphics::Mesh>();
  }
  void Update()
  {
  }
  void End()
  {
  }
};
} // namespace Systems
} // namespace Tortuga

#endif