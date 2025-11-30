#include "render_buffer.h"

static const RenderBuffer NULL_RENDER_BUFFER{ 0 };

RenderBuffer create_rb(GLenum format, int width, int height)
{
	RenderBuffer rb = { 0 };
	rb.Format = format;
	glGenRenderbuffers(1, &rb.Id);
	glBindRenderbuffer(GL_RENDERBUFFER, rb.Id);
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return rb;
}

void release_rb(RenderBuffer* rb)
{
	glDeleteRenderbuffers(1, &rb->Id);
}

bool rb_is_null(RenderBuffer& rb)
{
	return rb.Format == NULL_RENDER_BUFFER.Format && rb.Id == NULL_RENDER_BUFFER.Id;
}