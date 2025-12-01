#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4x4 View;
uniform mat4x4 Projection;

out vec3 vTexCoord;

void main()
{
    vec4 pos = Projection * View * vec4(aPos, 1.0);
    gl_Position = pos.xyww;

    vTexCoord = aPos; 
}