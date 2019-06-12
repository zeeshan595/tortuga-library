#ifndef _CORE_ENVIRONMENT
#define _CORE_ENVIRONMENT

#include <any>
#include <vector>

#include "./Entity.h"

namespace Tortuga {
namespace Core {
struct Environment {
  std::vector<Entity> Entities;
};

template <typename T> struct EntityData {
  Entity *entity;
  T Component;
};

std::vector<EntityData<std::any>> GetComponents(Environment env,
                                                std::type_index typeIndex) {
  std::vector<EntityData<std::any>> rtn;
  for (uint32_t i = 0; i < env.Entities.size(); i++) {
    rtn.push_back({&env.Entities[i], env.Entities[i].GetComponent(typeIndex)});
  }
}
} // namespace Core
} // namespace Tortuga

#endif