#version 330 core
#include "res/shaders/basic_material.glsl"
#include "res/shaders/lights.glsl"
out vec4 FragColor;

in vec3 vPos;
in vec3 vFragPos;
in vec3 vLocalPos;
in vec2 vTexCoord;

uniform Material uMaterial;

uniform vec3 uViewPosition;


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

uniform samplerCube uSkybox;

float uWavePeakScatterStrength = 0.7;
float uScatterStrength = 0.5;
float uScatterShadowStrength = 0.9;
float uBubbleDensity = 0.8;

vec3 uScatterColor = vec3(0.6, 0.8, 1.0) * 1.5;
float uSunIrradiance = 0.6;

vec3 uBubbleColor = vec3(1.0);

uniform float uFoamThreshold;
uniform float uFoamHardness;
uniform float uFoamIntensity;
uniform float uFoamDistanceFade;

uniform vec3 uFoamColor;

uniform vec3 uSeaColor;

uniform float uFogDistance;

vec3 water_lighting_simple(vec3 N, vec3 V, vec3 L, vec3 sunColor)
{
    
    // Fresnel
    float fresnel = 0.02 + 0.98 * pow(1.0 - max(dot(N, V), 0.0), 5.0);
    // Sun specular (very sharp)
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), 400.0);
    
    // Fake sky color (or sample cubemap)
    vec3 sky = vec3(0.6, 0.8, 1.0) * 0.5;
    vec3 I = normalize(vFragPos - V);
    vec3 R = reflect(I, normalize(N)); 
    sky = vec3(texture(uSkybox, R).rgb);
    return mix(uSeaColor, sky, fresnel * 0.7) + spec * sunColor * 3.0;
}


vec3 compute_normal_of_wave(vec3 pos)
{
    vec3 N = vec3(0.0);
    vec2 P = pos.xz;

    float a = uAmplitude;
    float w = uFrequency;
    float s = uSpeed;
    float seed = uInitialSeed;

    float sumX = 0.0;
    float sumY = 0.0;
    float sumZ = 0.0;

    for(int i = 0; i < uWaveCount; ++i)
    {
        float Q = uSteepness / (w * a * float(uWaveCount));
        vec2 d = normalize(vec2(cos(seed), sin(seed)));
        float k = w;
        float A = a;

        float phi = k * dot(d, P) + s * uTime;
        float C = cos(phi);
        float S = sin(phi);

        // Gerstner normal derivatives
        sumX += d.x * Q * A * k * C;
        sumY += A * k * S;
        sumZ += d.y * Q * A * k * C;

        // fractal wave update
        w *= uLacunarity;
        a *= uPersistance;
        s *= uSpeedRamp;
        seed += uSeedIter;
    }

    vec3 normal = normalize(vec3(
        -sumX,
        1 - sumY,
        -sumZ
    ));

    return normal;
}

float dot_clamped(vec3 a, vec3 b)
{
    return clamp(dot(a, b), 0.0, 1.0);
}

vec3 compute_light_scatter(vec3 L, vec3 V, vec3 N)
{
    float k1 = uWavePeakScatterStrength * pow(dot_clamped(L, -V), 4.0) * pow(0.5 - 0.5 * dot(L, N), 3.0);
    float k2 = uScatterStrength * pow(dot_clamped(V, N), 2.0);
    float k3 = uScatterShadowStrength * dot_clamped(N, L);
    float k4 = uBubbleDensity;

    vec3 scatter = (k1 + k2) * uScatterColor * uSunIrradiance;
    scatter += k3 * uScatterColor * uSunIrradiance + k4 * uBubbleColor * uSunIrradiance;

    return scatter;
}

void main()
{
    float totalWaveHeight = vLocalPos.y;
    vec3 normal = normalize(compute_normal_of_wave(vLocalPos));
    vec3 viewDir = normalize(uViewPosition - vFragPos);  // or from camera pos
    vec3 lightDir = normalize(-uDirectionalLight.Direction);

    vec3 baseWater = water_lighting_simple(normal, viewDir, lightDir, uDirectionalLight.DiffuseColor);
    vec3 scatter   = vec3(1.0);//compute_light_scatter(lightDir, viewDir, normal);

    // ======================
    // FOAM / WHITECAPS
    // ======================
    // 1. Steepness-based foam (Jacobian approximation via normal.y)
    float steepness = 1 - normal.y; // 0 = flat, 1 = vertical

    // 2. Height-based foam bias (higher waves = more likely to break)
    float heightFactor = totalWaveHeight / (uAmplitude * 2.0); // rough normalization

    // Combine
    float foamRaw = steepness;
    foamRaw = pow(foamRaw, 2.0); // make it appear only on real peaks

    // Smooth but sharp threshold
    float foamMask = smoothstep(uFoamThreshold, uFoamThreshold + 0.15, foamRaw);
    foamMask = pow(foamMask, uFoamHardness);

    // Fade foam with distance to avoid noisy far-away foam
    float dist = length(uViewPosition - vFragPos);
    foamMask *= clamp(1.0 - dist / uFoamDistanceFade, 0.0, 1.0);

    // Final foam contribution
    vec3 foam = uFoamColor * foamMask * uFoamIntensity * (scatter + uDirectionalLight.DiffuseColor);

    // Composite: water + scatter + foam on top
    vec3 color = baseWater * scatter;
    color = mix(color, foam, foamMask); // foam overrides everything where present

    // add fog
    vec3 I = normalize(vFragPos - uViewPosition);
    vec3 R = reflect(I, normal); 
    vec3 sky = vec3(texture(uSkybox, R).rgb);
    
    float fogDistance = distance(uViewPosition, vFragPos);
    float fogFactor = clamp(fogDistance / uFogDistance, 0.0, 1.0);

    color = mix(color, sky, fogFactor);

    FragColor = vec4(color , 1.0);
} 