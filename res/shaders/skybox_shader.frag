#version 330 core
#include "res/shaders/basic_material.glsl"
#include "res/shaders/lights.glsl"
out vec4 FragColor;

in vec3 vTexCoord;

uniform samplerCube uSkybox;

void main()
{
    FragColor = vec4(texture(uSkybox, normalize(vTexCoord)).rgb, 1.0);
} 