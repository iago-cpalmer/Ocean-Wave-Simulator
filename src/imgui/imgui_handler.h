
#ifndef IMGUI_HANDLER_H
#define IMGUI_HANDLER_H

#include <functional>
#include <string>
#include <variant>
#include <glm/glm.hpp>

enum class ImGuiComponentType
{
    Float,
    Int,
    Vec3,
    Vec2,
    Color
};

typedef struct
{
    float minValue;
    float maxValue;
    float currentValue;
    std::function<void(float)> callback;
} FloatComponent;

typedef struct
{
    int minValue;
    int maxValue;
    int currentValue;
    std::function<void(int)> callback;
} IntComponent;

typedef struct
{
    glm::vec2 minValue;
    glm::vec2 maxValue;
    glm::vec2 currentValue;
    std::function<void(glm::vec2)> callback;
} Vec2Component;

typedef struct
{
    glm::vec3 minValue;
    glm::vec3 maxValue;
    glm::vec3 currentValue;
    std::function<void(glm::vec3)> callback;
} Vec3Component;

typedef struct
{
    // Color is represented as RGB
    glm::vec3 currentColor;
    std::function<void(glm::vec3)> callback;
} ColorComponent;

typedef struct
{
    std::string Name;
    ImGuiComponentType Type;
    std::variant<FloatComponent, IntComponent, Vec3Component, Vec2Component, ColorComponent> Data;
    
} ImGuiComponent;

void imgui_init();
void imgui_render();
void imgui_finish_render();
void imgui_terminate();
void imgui_add_component(ImGuiComponent *comp);
bool imgui_has_cursor();
#endif