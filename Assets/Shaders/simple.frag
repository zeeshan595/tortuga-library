#version 460
#define MAXIMUM_LIGHT_INFOS 10

struct LightInfo
{
  vec4 position;
  vec4 forward;
  vec4 color;
  int type;
  float intensity;
  float range;
};
layout(set = 2, binding = 0) readonly uniform LightInfoStruct
{
  uint lightsAmount;
  uint lightsReserved1;
  uint lightsReserved2;
  uint lightsReserved3;
  LightInfo lights[10];
};

layout(location = 0) in vec3 surfaceNormal;
layout(location = 1) in vec3 cameraVector;
layout(location = 2) in vec3 lightVectors[MAXIMUM_LIGHT_INFOS];

layout(location = 0) out vec4 outColor;

void main()
{
  vec3 uintNormal = normalize(surfaceNormal);
  vec3 unitCameraVector = normalize(cameraVector);

  vec3 diffuse = vec3(0.02);
  vec3 specular = vec3(0.);

  for (uint i = 0; i < 1; i++)
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

  outColor = vec4(diffuse + specular, 1.);
}