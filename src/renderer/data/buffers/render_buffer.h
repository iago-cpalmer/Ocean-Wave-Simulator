#include <glad/glad.h>
#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

typedef struct
{
	unsigned int Id;
	GLenum Format;
} RenderBuffer;


RenderBuffer create_rb(GLenum format, int width, int height);

void release_rb(RenderBuffer* rb);

bool rb_is_null(RenderBuffer& rb);
#endif // !RENDER_BUFFER_H


