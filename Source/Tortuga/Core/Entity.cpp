#include "./Entity.hpp"

namespace Tortuga
{
namespace Core
{
namespace Entity
{
auto environment = Environment();
Environment::~Environment()
{
  for (auto i = environment.Entities.begin(); i < environment.Entities.end(); i++)
    delete *i;
    
  environment.Entities.clear();
}
Entity *Create()
{
  auto temp = new Entity();
  environment.Entities.push_back(temp);
  return temp;
}
void Destroy(Entity *data)
{
  for (auto i = environment.Entities.begin(); i < environment.Entities.end(); i++)
  {
    if (*i == data)
    {
      environment.Entities.erase(i);
      break;
    }
  }
}

std::vector<Entity *> GetAllEntities()
{
  return environment.Entities;
}
} // namespace Entity
} // namespace Core
} // namespace Tortuga