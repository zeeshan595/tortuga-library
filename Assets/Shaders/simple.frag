#version 450
#extension GL_ARB_separate_shader_objects : enable
#include "./Partial/lightInfo.comp"

layout(set = 1, binding = 0) uniform Light {
  uint LightsSize;
  uint LightsReserved1;
  uint LightsReserved2;
  uint LightsReserved3;
  LightInfo lights[LIGHTS_AMOUNT];
};
layout(set = 2, binding = 0) uniform sampler2D albedo;

layout(location = 0) in vec3 surfaceNormal;
layout(location = 1) in vec3 cameraVector;
layout(location = 2) in vec2 textureCoords;
layout(location = 3) in vec3 lightVectors[LIGHTS_AMOUNT];

layout(location = 0) out vec4 outColor;

void main() {
  vec3 uintNormal = normalize(surfaceNormal);
  vec3 unitCameraVector = normalize(cameraVector);

  vec3 diffuse = vec3(0.02);
  vec3 specular = vec3(0.);

  for (uint i = 0; i < LightsSize; i++)
  {
    if (lightVectors[i] != vec3(0.))
    {
      vec3 unitLightVector = normalize(lightVectors[i]);
      //diffuse
      float diffuseFactor = dot(uintNormal, unitLightVector);
      diffuseFactor = max(diffuseFactor, 0.);
      diffuse += diffuseFactor * lights[i].color.rgb;

      //specular
      vec3 reflectedLightVector = reflect(-unitLightVector, uintNormal);
      float specularFactor = dot(reflectedLightVector, cameraVector);
      specularFactor = max(specularFactor, 0.);
      specular += specularFactor * lights[i].color.rgb * 0.05;
    }
  }

  outColor = texture(albedo, textureCoords);//vec4(diffuse + specular, 1.);
}