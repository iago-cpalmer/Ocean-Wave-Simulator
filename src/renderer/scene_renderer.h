#ifndef SCENE_RENDERER_H
#define SCENE_RENDERER_H

#include "renderer.h"
#include "camera.h"
#include "data/mesh.h"
#include "light/point_light.h"
#include "light/directional_light.h"
#include "../scene/scene.h"

void begin_render(Scene* rp_Scene, CameraInfo* rpCamera);
void end_render();

void draw_mesh(Mesh* rp_mesh, glm::mat4x4 r_transform, MaterialHandle* r_materials, unsigned int r_matCount);

static void _calculate_view_matrix();
static void _calculate_projection_matrix();

void write_point_light_to_shader(Shader* rpShader, PointLight* rp_Light);
void write_directional_light_to_shader(Shader* rpShader, DirectionalLight* rp_Light);

void write_material_to_shader(Material* rp_material);

#endif // !SCENE_RENDERER_H


