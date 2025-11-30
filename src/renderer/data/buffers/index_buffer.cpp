#include "index_buffer.h"

IndexBuffer create_ibo(const unsigned int r_size_bytes, const void* rp_data, const GLenum r_usage)
{
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, r_size_bytes, rp_data, r_usage);

    return IndexBuffer{ ibo };
}

void ibo_set_data(const IndexBuffer& r_ibo, const void* rp_data, const GLsizeiptr r_size_bytes, const GLintptr r_offset)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_ibo.Id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, r_offset, r_size_bytes, rp_data);
}

void release_ibo(IndexBuffer& r_ibo)
{
    glDeleteBuffers(1, &r_ibo.Id);
    r_ibo.Id = 0;
}
