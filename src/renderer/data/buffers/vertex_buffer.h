#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

typedef struct
{
	unsigned int Id;
} VertexBuffer;

/// <summary>
/// Create vertex buffer
/// </summary>
/// <param name="r_size_bytes">Size in bytes to be allocated</param>
/// <param name="rp_data">Data to be set on init. If nullptr, buffer will be "empty"</param>
/// <param name="r_usage">Set usage of buffer</param>
/// <returns>Index buffer</returns>
VertexBuffer create_vbo(const unsigned int r_size_bytes, const void* rp_data, const GLenum r_usage);

/// <summary>
/// Set data without allocating and ensuring synchronization
/// </summary>
/// <param name="r_vbo">Vertex buffer to set data to</param>
/// <param name="rp_data">Data to set</param>
/// <param name="r_size_bytes">Size in bytes of data</param>
/// <param name="r_offset">Offset to write from</param>
void vbo_set_data(const VertexBuffer& r_vbo, const void* rp_data, const GLsizeiptr r_size_bytes, const GLintptr r_offset);

inline void vbo_bind(const VertexBuffer& r_vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, r_vbo.Id);
}

inline void vbo_unbind(const VertexBuffer& r_vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/// <summary>
/// Deletes buffer
/// </summary>
/// <param name="r_vbo">Buffer to delete. Its Id will be set to 0</param>
void release_vbo(VertexBuffer& r_vbo);

#endif // !VERTEX_BUFFER_H


