#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "./Partial/lightInfo.comp"

layout(set = 0, binding = 0) uniform MeshModel {
  mat4 model;
};

layout(set = 1, binding = 0) uniform CameraProperties {
  mat4 view;
  mat4 projection;
};

layout(set = 2, binding = 0) uniform Light {
  uint LightsSize;
  uint LightsReserved1;
  uint LightsReserved2;
  uint LightsReserved3;
  LightInfo lights[LIGHTS_AMOUNT];
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexture;

layout(location = 0) out vec3 surfaceNormal;
layout(location = 1) out vec3 cameraVector;
layout(location = 2) out vec2 texture;
layout(location = 3) out vec3 lightVectors[LIGHTS_AMOUNT];

void main() {
  vec4 worldPosition = model * vec4(inPosition, 1.0);
  surfaceNormal = (model * vec4(inNormal, 0.0)).xyz;
  cameraVector = (inverse(view) * vec4(0., 0., 0., 1.)).xyz - worldPosition.xyz;
  texture = inTexture;

  for (uint i = 0; i < LightsSize; i++)
  {
    if (lights[i].forward != vec4(0.))
      lightVectors[i] = lights[i].position.xyz - inPosition;
  }

  gl_Position = projection * view * worldPosition;
}