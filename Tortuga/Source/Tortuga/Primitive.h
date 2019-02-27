#ifndef _TORTUGA_PRIMITIVE
#define _TORTUGA_PRIMITIVE

#include "Core.h"

enum PrimitiveType
{
  PRIMITIVE_SPHERE,
  PRIMITIVE_BOX,
  PRIMITIVE_TORUS,
  PRIMITIVE_CYLINDER,
  PRIMITIVE_CONE,
  PRIMITIVE_PLANE,
  PRIMITIVE_HEXAGONAL,
  PRIMITIVE_CAPSULE
};
enum PrimitiveMethod
{
  PRIMITIVE_METHOD_UNION,
  PRIMITIVE_METHOD_SUBTRACT,
  PRIMITIVE_METHOD_INTERSECTION,
  PRIMITIVE_METHOD_UNION_SMOOTH,
  PRIMITIVE_METHOD_SUBTRACT_SMOOTH,
  PRIMITIVE_METHOD_INTERSECTION_SMOOTH
};
struct SmoothInfo
{
  bool Enabled = false;
  float Amount = 0;
};
struct OnionInfo
{
  bool Enabled = false;
  std::vector<float> Slices = {};
};
struct RevolutionInfo
{
  bool Enabled = false;
  float Thickness = 0;
  glm::vec3 Point = glm::vec3(0, 0, 0);
};
struct ExtrusionInfo
{
  bool Enabled = false;
  float Amount = 0;
};
enum SymmetryType
{
  SYMMETRY_TYPE_X,
  SYMMETRY_TYPE_Y,
  SYMMETRY_TYPE_Z,
};
struct SymmetryInfo
{
  bool Enabled = false;
  SymmetryType Type;
  glm::vec3 Mirror = {0, 0, 0};
};
struct RepetitionInfo
{
  bool Enabled = false;
  glm::vec3 Distance = {1, 1, 1};
};
struct DisplacementInfo
{
  bool Enabled = false;
  float Amount = 0;
};
enum BendType
{
  BEND_TYPE_X,
  BEND_TYPE_Y,
  BEND_TYPE_Z,
};
struct BendInfo
{
  bool Enabled = false;
  BendType Type = BEND_TYPE_Y;
  float Amount = 0;
};
struct PrimitiveAlterations
{
  glm::vec3 Elongate = glm::vec3(0, 0, 0);
  SmoothInfo Smoothness = SmoothInfo();
  OnionInfo Onion = OnionInfo();
  ExtrusionInfo Extrusion = ExtrusionInfo();
  RevolutionInfo Revolution = RevolutionInfo();
  SymmetryInfo Symmetry = SymmetryInfo();
  RepetitionInfo Repetition = RepetitionInfo();
  DisplacementInfo Displacement = DisplacementInfo();
  BendInfo Bend = BendInfo();
};
struct Primitive
{
  //Transformation
  glm::vec3 Position = {0, 0, 0};
  glm::vec4 Rotation = {0, 0, 0, 0};
  glm::vec3 Scale = {1, 1, 1};

  //Properties
  PrimitiveType Type = PRIMITIVE_SPHERE;
  PrimitiveMethod Method = PRIMITIVE_METHOD_UNION;
  PrimitiveAlterations Alterations = PrimitiveAlterations();
};

#endif