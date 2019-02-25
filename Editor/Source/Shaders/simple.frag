#version 450
#extension GL_ARB_separate_shader_objects : enable

#define ResolutionX 1024
#define ResolutionY 768

layout(location = 0) out vec4 outColor;

void main() {
    vec2 fragmentPosition = vec2(gl_FragCoord.x / ResolutionX, gl_FragCoord.y / ResolutionY);
    vec3 rayDirection = normalize(vec3(fragmentPosition.x, fragmentPosition.y, 1));
    
    outColor = vec4(rayDirection, 1.0);
}