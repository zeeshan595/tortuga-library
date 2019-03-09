#version 450

#define ResolutionX 1024
#define ResolutionY 768
#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST .01

layout(location=0)out vec4 outColor;

float SphereDist(vec3 position, float radius, vec3 point)
{
    return length(point-position.xyz)-radius;
}

float GetMinDistance(vec3 point)
{
    return SphereDist(vec3(0., 1., 5.5), 2., point);
}

float RayMarch(vec3 rayOrigin,vec3 rayDirection)
{
    float distanceFromOrigin=0.;
    for(int i=0;i<MAX_STEPS;i++)
    {
        vec3 rayLocation=rayOrigin+(distanceFromOrigin*rayDirection);
        float dist=GetMinDistance(rayLocation);
        distanceFromOrigin+=dist;
        if(distanceFromOrigin<SURF_DIST||distanceFromOrigin>MAX_DIST)break;
    }
    return distanceFromOrigin;
}

void main()
{
    vec2 uv=(gl_FragCoord.xy-.5*vec2(ResolutionX,ResolutionY))/ResolutionY;
    
    vec3 rayOrigin=vec3(0,1,0);
    vec3 rayDirection=normalize(vec3(uv,1.));
    
    float d=RayMarch(rayOrigin,rayDirection);
    d/=5.;
    vec3 col=vec3(d);
    outColor=vec4(col,1.);
}

