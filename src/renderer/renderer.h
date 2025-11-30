#ifndef RENDERER_H
#define RENDERER_H

#include "../core/window.h"
#include "../renderer/camera.h"
#include <glm/ext/matrix_float4x4.hpp>

#include "data/buffers/index_buffer.h"
#include "data/buffers/vertex_array.h"

//#define WIREFRAME_MODE

typedef struct
{
	glm::vec2 ViewportSizePx;
} RenderData;

enum class FaceCullingType
{
	NONE = 0,
	BACK,
	FRONT,
	FRONT_AND_BACK
};

const RenderData& get_renderer_data();

void renderer_init_opengl();

void renderer_set_clear_color_normalized(float r, float g, float b);
void renderer_set_clear_color(float r, float g, float b);

/// <summary>
/// Sets face culling type. By default enabled and set to cull only back faces
/// </summary>
/// <param name="type"></param>
void renderer_set_face_culling_type(FaceCullingType type);

void renderer_prepare_frame();
void renderer_finish_render();

void renderer_change_viewport_callback(int rWidth, int rHeight);


void draw_indexed(const VertexArray& r_vao, const IndexBuffer& r_ibo, unsigned int r_indexCount, unsigned int r_startIndex);

#if _DEBUG
void APIENTRY gl_debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif

#endif // !RENDERER_H

