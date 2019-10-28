#ifndef _LIGHT_COMPONENT
#define _LIGHT_COMPONENT

#include <glm/glm.hpp>

#include "../Core/ECS/Entity.hpp"

namespace Tortuga
{
namespace Components
{
enum LightType
{
  POINT,
  DIRECTIONAL
};
struct Light : Core::ECS::Component
{
private:
  LightType Type = LightType::POINT;
  glm::vec4 Color = glm::vec4(1, 1, 1, 1);
  float Intensity = 1.0f;
  float Range = 10.0f;

public:
  LightType GetType()
  {
    return this->Type;
  }
  void SetType(LightType type)
  {
    this->Type = type;
  }

  glm::vec4 GetColor()
  {
    return this->Color;
  }
  void SetColor(glm::vec4 color)
  {
    this->Color = color;
  }

  float GetIntensity()
  {
    return this->Intensity;
  }
  void SetIntensity(float intensity)
  {
    this->Intensity = intensity;
  }

  float GetRange()
  {
    return this->Range;
  }
  void SetRange(float range)
  {
    this->Range = range;
  }
};
} // namespace Components
} // namespace Tortuga

#endif