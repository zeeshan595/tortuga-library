#version 460

#define MAXIMUM_LIGHT_INFOS 10

layout(set = 0, binding = 0) readonly uniform CameraTransform
{
  mat4 view;
  mat4 projection;
};
layout(set = 1, binding = 0) readonly uniform MeshTransform
{
  mat4 model;
};
struct LightInfo
{
  vec4 Color;
  int Type;
  float Intensity;
  float Range;
};
layout(set = 2, binding = 0) readonly uniform LightInfoStruct
{
  uint lightsAmount;
  uint lightsReserved1;
  uint lightsReserved2;
  uint lightsReserved3;
  LightInfo lights[10];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexture;
layout(location = 2) in vec3 inNormal;

void main()
{
  gl_Position = projection * view * model * vec4(inPosition, 1.);
}