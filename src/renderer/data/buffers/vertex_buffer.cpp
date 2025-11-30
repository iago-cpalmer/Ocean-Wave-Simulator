#include "vertex_buffer.h"

#include <glad/glad.h>

VertexBuffer create_vbo(const unsigned int r_size_bytes, const void* rp_data, const GLenum r_usage)
{
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, r_size_bytes, rp_data, r_usage);

    return VertexBuffer{ vbo };
}

void vbo_set_data(const VertexBuffer& r_vbo, const void* rp_data, const GLsizeiptr r_size_bytes, const GLintptr r_offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, r_vbo.Id);
    glBufferSubData(GL_ARRAY_BUFFER, r_offset, r_size_bytes, rp_data);
}

void release_vbo(VertexBuffer& r_vbo)
{
    glDeleteBuffers(1, &r_vbo.Id);
    r_vbo.Id = 0;
}
