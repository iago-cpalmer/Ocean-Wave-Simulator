#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_buffer_layout.h"

typedef struct
{
	unsigned int Id;
} VertexArray;

/// <summary>
/// Create vertex array object
/// </summary>
/// <param name="r_size_bytes">Size in bytes to be allocated</param>
/// <param name="rp_data">Data to be set on init. If nullptr, buffer will be "empty"</param>
/// <param name="r_usage">Set usage of buffer</param>
/// <returns>Index buffer</returns>
VertexArray create_vao();

void vao_add_vbo(const VertexArray& r_vao, const VertexBuffer& r_vbo, const VertexBufferLayout& r_vertexLayout);
void vao_add_ibo(const VertexArray& r_vao, const IndexBuffer& r_ibo);


inline void vao_bind(const VertexArray& r_vao)
{
    glBindVertexArray(r_vao.Id);
}

inline void vao_unbind(const VertexArray& r_vao)
{
    glBindVertexArray(0);
}


/// <summary>
/// Deletes buffer
/// </summary>
/// <param name="r_vao">Buffer to delete. Its Id will be set to 0</param>
void release_vao(VertexArray& r_vao);

#endif // !VERTEX_ARRAY_H


