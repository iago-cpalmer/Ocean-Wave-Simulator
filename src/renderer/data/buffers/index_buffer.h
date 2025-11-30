#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>

typedef struct
{
	unsigned int Id;
} IndexBuffer;

/// <summary>
/// Create index buffer
/// </summary>
/// <param name="r_size_bytes">Size in bytes to be allocated</param>
/// <param name="rp_data">Data to be set on init. If nullptr, buffer will be "empty"</param>
/// <param name="r_usage">Set usage of buffer</param>
/// <returns>Index buffer</returns>
IndexBuffer create_ibo(const unsigned int r_size_bytes, const void* rp_data, const GLenum r_usage);

/// <summary>
/// Set data without allocating and ensuring synchronization
/// </summary>
/// <param name="r_ibo">Index buffer to set data to</param>
/// <param name="rp_data">Data to set</param>
/// <param name="r_size_bytes">Size in bytes of data</param>
/// <param name="r_offset">Offset to write from</param>
void ibo_set_data(const IndexBuffer& r_ibo, const void* rp_data, const GLsizeiptr r_size_bytes, const GLintptr r_offset);

inline void ibo_bind(const IndexBuffer& r_vbo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_vbo.Id);
}

inline void ibo_unbind(const IndexBuffer& r_vbo)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/// <summary>
/// Deletes buffer
/// </summary>
/// <param name="r_ibo">Buffer to delete. Its Id will be set to 0</param>
void release_ibo(IndexBuffer& r_ibo);

#endif // !INDEX_BUFFER_H
