#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "./Partial/lightInfo.comp"

layout(set = 1, binding = 0) uniform Light {
  LightInfo lights[];
};

layout(location = 0) in vec3 surfaceNormal;
layout(location = 1) in vec3 lightVectors;
layout(location = 2) in vec3 cameraVector;

layout(location = 0) out vec4 outColor;

void main() {
  vec3 uintNormal = normalize(surfaceNormal);
  vec3 unitLightVector = normalize(lightVectors);
  vec3 unitCameraVector = normalize(cameraVector);

  //diffuse
  float diffuseFactor = dot(uintNormal, unitLightVector);
  diffuseFactor = max(diffuseFactor, 0.);
  vec3 diffuse = vec3(0.01, 0.01, 0.01) + diffuseFactor * lights[0].color.rgb;

  //specular
  vec3 reflectedLightVector = reflect(-unitLightVector, uintNormal);
  float specularFactor = dot(reflectedLightVector, cameraVector);
  specularFactor = max(specularFactor, 0.);
  float dampedSpecFactor = pow(specularFactor, 10.);
  vec3 specular = dampedSpecFactor * lights[0].color.rgb;

  outColor = vec4(diffuse + specular, 1.);
}