#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "./Partial/lightInfo.comp"

layout(set = 0, binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 projection;
} ubo;

layout(set = 1, binding = 0) uniform Light {
  LightInfo lights[];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTexture;

layout(location = 0) out vec3 surfaceNormal;
layout(location = 1) out vec3 lightVectors;

void main() {
  gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPosition, 1.0);
  surfaceNormal = (ubo.model * vec4(inNormal, 0.0)).xyz;
  lightVectors = lights[0].position.xyz - inPosition.xyz;
}