#include "frame_buffer.h"
#include "../../../core/ErrorHandler.h"

#include "../../../assets/public/assets_handler.h"
#include "../../../core/memory_utils.h"

FrameBuffer create_fb(FbRenderTargetType type, const int width, const int height, const int nChannels)
{
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	FrameBuffer fb = FrameBuffer{ fbo };
	fb.RenderTargetType = type;
	fb.DepthTarget = { 0 };
	switch (type)
	{
		case FbRenderTargetType::Texture:
		{
			// Create texture
			Texture* texture = (Texture*)CE_MALLOC(sizeof(Texture));
			create_texture(texture, nChannels, width, height);
			fb.ColorTarget.Texture = ah_register_texture(texture);

			// Attach texture
			use_fb(fb);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->Texture, 0);
		} break;
		case FbRenderTargetType::RenderBuffer:
		{
			fb.ColorTarget.RenderBuffer = create_rb(GL_RGBA8, width, height);
			_fb_attach_rb(&fb, &fb.ColorTarget.RenderBuffer);
		} break;
	}
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		THROW_ERROR("ERROR::create_fb - Frame buffer was not created correctly");
	}

	fb_unbind();

	return fb;
}
void fb_create_rb_for_depth(FrameBuffer* fb, GLenum format, int width, int height)
{
	fb->DepthTarget = create_rb(format, width, height);
	_fb_attach_rb(fb, &fb->DepthTarget);
}

void _fb_attach_rb(FrameBuffer* fb, RenderBuffer* rb)
{
	use_fb(*fb);
	switch (rb->Format)
	{
		case GL_DEPTH:
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb->Id);
			break;
		case GL_DEPTH24_STENCIL8:
		case GL_DEPTH32F_STENCIL8:
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb->Id);
			break;
		case GL_RGBA8:
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb->Id);
			break;
		default:
			THROW_ERROR("ERROR::fb_attach_rb - Not supported format of render buffer object");
	}
	fb_unbind();
}

void release_fb(FrameBuffer& r_fb)
{
	glDeleteFramebuffers(1, &r_fb.Id);

	// Release attached color buffer 
	switch (r_fb.RenderTargetType)
	{
		case FbRenderTargetType::Texture:
		{
			Texture* texture = ah_get_texture(r_fb.ColorTarget.Texture);
			if (texture != nullptr)
			{
				release_texture(texture);
				ah_unregister_texture(r_fb.ColorTarget.Texture);
				CE_DEALLOC(texture);
			}
		} break;
		case FbRenderTargetType::RenderBuffer:
		{
			release_rb(&r_fb.ColorTarget.RenderBuffer);
		} break;
	}
	
	// Release depth buffer (if any)
	if (!rb_is_null(r_fb.DepthTarget))
	{
		release_rb(&r_fb.DepthTarget);
	}

}
