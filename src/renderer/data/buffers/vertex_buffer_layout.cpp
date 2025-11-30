#include "vertex_buffer_layout.h"

unsigned int get_size_of_type(VertexAttributeType r_type)
{
	switch (r_type)
	{
		case VertexAttributeType::FLOAT: return sizeof(float);
		case VertexAttributeType::UINT: return sizeof(unsigned int);
		case VertexAttributeType::INT: return sizeof(int);
		case VertexAttributeType::BOOL: return sizeof(bool);
		case VertexAttributeType::DOUBLE: return sizeof(double);
	}
}

GLenum get_gl_type(VertexAttributeType r_type)
{
	switch (r_type)
	{
		case VertexAttributeType::FLOAT: return GL_FLOAT;
		case VertexAttributeType::UINT: return GL_UNSIGNED_INT;
		case VertexAttributeType::INT: return GL_INT;
		case VertexAttributeType::BOOL: return GL_BOOL;
		case VertexAttributeType::DOUBLE: return GL_DOUBLE;
	}
}

VertexBufferLayout create_vertex_buffer_layout(VertexAttribute* rp_vertexAttributes, const  unsigned int r_vertexAttrCount)
{
	unsigned int stride = 0;
	for (unsigned int i = 0; i < r_vertexAttrCount; i++)
	{
		stride += get_size_of_type(rp_vertexAttributes[i].Type) * rp_vertexAttributes[i].Count;
	}

	return VertexBufferLayout{ rp_vertexAttributes, r_vertexAttrCount, stride };
}