#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "./Partial/lightInfo.comp"

layout(set = 1, binding = 0) uniform Light {
  LightInfo lights[];
};

layout(location = 0) in vec3 surfaceNormal;
layout(location = 1) in vec3 lightVectors;

layout(location = 0) out vec4 outColor;

void main() {
  vec3 uintNormal = normalize(surfaceNormal);
  vec3 unitLightVector = normalize(lightVectors);

  float nDot = dot(uintNormal, unitLightVector);
  float brightness = max(nDot, 0.0);
  vec3 diffuse = brightness * lights[0].color.rgb;

  outColor = vec4(diffuse, 1.);
}