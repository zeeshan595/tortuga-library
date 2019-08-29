#version 450
#extension GL_ARB_separate_shader_objects : enable

struct LightInfo
{
  vec4 position;
  vec4 forward;
  vec4 color;
  uint type;
  float intensity;
  float range;
};

layout(set = 1, binding = 0) uniform Light {
  LightInfo lights[];
};

layout(location = 0) out vec4 outColor;

void main() {
    outColor = lights[0].color;
}