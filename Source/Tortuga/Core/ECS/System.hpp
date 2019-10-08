#ifndef _ECS_SYSTEM
#define _ECS_SYSTEM

namespace Tortuga
{
namespace Core
{
namespace ECS
{
struct System
{
public:
  System(){}
  virtual ~System() {} //allow proper inheritance
  virtual void Update() {}
};
} // namespace ECS
} // namespace Core
} // namespace Tortuga

#endif