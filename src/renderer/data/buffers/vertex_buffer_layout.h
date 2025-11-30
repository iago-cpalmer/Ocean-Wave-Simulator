#include <glad/glad.h>
#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

enum class VertexAttributeType
{
	FLOAT,
	INT,
	UINT,
	BOOL,
	DOUBLE
};

// (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
typedef struct
{
	VertexAttributeType Type;
	unsigned int Count;
} VertexAttribute;

typedef struct {
	VertexAttribute* VertexAttributes;
	unsigned int VertexAttributeCount;

	unsigned int Stride;

} VertexBufferLayout;

VertexBufferLayout create_vertex_buffer_layout(VertexAttribute* rp_vertexAttributes, const  unsigned int r_vertexAttrCount);


// Utility functions
unsigned int get_size_of_type(VertexAttributeType r_type);
GLenum get_gl_type(VertexAttributeType r_type);

#endif // !VERTEX_BUFFER_LAYOUT_H
