#include "./Entity.hpp"

namespace Tortuga
{
namespace Core
{
namespace Entity
{
Entity::Entity()
{
  this->GUID = Core::GUID::GenerateGUID();
}
Entity::~Entity()
{
  if (this->Components.size() > 0)
    Console::Warning("Please remove all components before destroying the entity");
}
std::string Entity::GetGUID()
{
  return this->GUID;
}
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
      delete data;
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