#version 330 core
#include "res/shaders/transforms.glsl"

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

out vec3 vPos;
out vec3 vFragPos;
out vec3 vLocalPos;
out vec2 vTexCoord;

uniform int uWaveCount;
uniform vec2 uWaveDirection;
uniform float uSpeed;
uniform float uTime;
uniform float uSteepness;

uniform float uFrequency;
uniform float uAmplitude;
uniform float uPersistance;
uniform float uLacunarity;
uniform float uInitialSeed;
uniform float uSeedIter;
uniform float uSpeedRamp;

vec3 get_wave_pos(vec3 worldPos)
{
    vec3 vpos = vec3(worldPos.x, 0.0, worldPos.z);
    vec2 coord = worldPos.xz;
    float h = 0.0;
    float a = 0.5;
    float w = 0.4;
    for(int i = 0; i < uWaveCount; ++i)
    {
        float W = a * sin(dot(uWaveDirection, coord) * w + uTime * uSpeed);

        h += W;
    }
    vpos.y = h;
    return vpos;
}

vec3 get_grestner_wave_pos(vec3 worldPos) 
{ 
    vec3 vpos = vec3(worldPos.x, 0.0, worldPos.z); 
    vec2 coord = worldPos.xz; 
    float a = uAmplitude; 
    float w = uFrequency; 
    float s = uSpeed; 
    float seed = uInitialSeed; 

    for(int i = 0; i < uWaveCount; ++i) 
    { 
        float angle = (i + 1.0) * 1.6180339887 * seed;
        vec2 d = vec2(cos(angle), sin(angle));
        float q = uSteepness / (w*a*uWaveCount); 

        float x = w * dot(d, coord) + uTime * s;

        vpos.x += q * a * d.x * cos(x); 
        vpos.z += q * a * d.y * cos(x);

        float wave = a * sin(x);
        vpos.y +=  wave;

        float dx = wave * cos(x) * a;
        
        w *= uLacunarity; 
        a *= uPersistance; 
        s *= uSpeedRamp; 
        seed += uSeedIter; 
    } 
        
    return vpos;  
}
void main()
{
    vec3 pos = get_grestner_wave_pos( aPos);

    vLocalPos = pos;
    VertexPosition vPositions = get_vertex_positions(pos);
    vFragPos = vPositions.WS_Position;
    vPos = vPositions.VS_Position;
    gl_Position = vPositions.CS_Position;

    vTexCoord = aTexCoord; 
}