#include "imgui_handler.h"

#include "imgui.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "core/window.h"

static std::vector<ImGuiComponent *> s_Components;

void imgui_init()
{
    ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(get_window_ptr(), true);
	ImGui_ImplOpenGL3_Init();

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8f, 0.3f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
}

void imgui_render()
{
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

   ImGui::Begin("Ocean parameters");

   for (auto *c : s_Components)
   {
        if(c == nullptr) continue;
        switch (c->Type)
        {
        case ImGuiComponentType::Float:
        {
            auto &d = std::get<FloatComponent>(c->Data);
            if (ImGui::DragFloat(c->Name.c_str(), &d.currentValue, (d.maxValue - d.minValue) / 100.0f, d.minValue, d.maxValue))
                d.callback(d.currentValue);
            break;
        }
        case ImGuiComponentType::Int:
        {
            auto &d = std::get<IntComponent>(c->Data);
            if (ImGui::DragInt(c->Name.c_str(), &d.currentValue, 1, d.minValue, d.maxValue))
                d.callback(d.currentValue);
            break;
        }
        case ImGuiComponentType::Vec3:
        {
            auto &d = std::get<Vec3Component>(c->Data);
            if (ImGui::DragFloat3(c->Name.c_str(), glm::value_ptr(d.currentValue), 0.1f))
                d.callback(d.currentValue);
            break;
        }
        case ImGuiComponentType::Vec2:
        {
            auto &d = std::get<Vec2Component>(c->Data);
            if (ImGui::DragFloat2(c->Name.c_str(), glm::value_ptr(d.currentValue), 0.1f))
                d.callback(d.currentValue);
            break;
        }
        case ImGuiComponentType::Color:
        {
            auto &d = std::get<ColorComponent>(c->Data);
            float col[3] = { d.currentColor.r, d.currentColor.g, d.currentColor.b };
            if (ImGui::ColorEdit3(c->Name.c_str(), col))
            {
                d.currentColor = glm::vec3(col[0], col[1], col[2]);
                d.callback(d.currentColor);
            }
            break;
        }
        default:
            break;
        }
   }

   ImGui::End();
}

void imgui_finish_render()
{
    ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_terminate()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void imgui_add_component(ImGuiComponent *comp)
{
    s_Components.push_back(comp);
}


bool imgui_has_cursor()
{
    ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse;
}