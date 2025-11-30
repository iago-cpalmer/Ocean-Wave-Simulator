#include "vertex_array.h"

#include <glad/glad.h>
#include "vertex_buffer_layout.h"
#include <iostream>

VertexArray create_vao()
{
    unsigned int vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return VertexArray{ vao };
}

void vao_add_vbo(const VertexArray& r_vao, const VertexBuffer& r_vbo, const VertexBufferLayout& r_vertexLayout)
{
    glBindVertexArray(r_vao.Id);
    glBindBuffer(GL_ARRAY_BUFFER, r_vbo.Id);

    int offset_bytes = 0;
    for (unsigned int i = 0; i < r_vertexLayout.VertexAttributeCount; i++)
    {
        unsigned int size_bytes = get_size_of_type(r_vertexLayout.VertexAttributes[i].Type) * r_vertexLayout.VertexAttributes[i].Count;
        glVertexAttribPointer(i,
            r_vertexLayout.VertexAttributes[i].Count,
            get_gl_type(r_vertexLayout.VertexAttributes[i].Type),
            GL_FALSE, 
            r_vertexLayout.Stride,
            (void*)offset_bytes
        );
        glEnableVertexAttribArray(i);
        offset_bytes += size_bytes;
    }
}

void vao_add_ibo(const VertexArray& r_vao, const IndexBuffer& r_ibo)
{
    glBindVertexArray(r_vao.Id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_ibo.Id);
}

void release_vao(VertexArray& r_vao)
{
    glDeleteBuffers(1, &r_vao.Id);
    r_vao.Id = 0;
}
