#version 460

layout(set = 0, binding = 0) readonly uniform CameraTransform
{
  mat4 view;
  mat4 projection;
};
layout(set = 1, binding = 0) readonly uniform MeshTransform
{
  mat4 model;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexture;
layout(location = 2) in vec3 inNormal;

void main() 
{
  gl_Position = vec4(inPosition, 1.) * model * view * projection;
}