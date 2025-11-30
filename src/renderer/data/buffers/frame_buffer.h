#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H


#include <glad/glad.h>
#include "../texture.h"
#include "render_buffer.h"

enum class FbRenderTargetType
{
    Texture,
    RenderBuffer
};

typedef struct
{
    unsigned int Id;

    FbRenderTargetType RenderTargetType;
    union
    {
        TextureHandle Texture;
        RenderBuffer RenderBuffer;
    } ColorTarget;
    RenderBuffer DepthTarget;
    
} FrameBuffer;

/// <summary>
/// Create vertex buffer
/// </summary>
/// <param name="r_size_bytes">Size in bytes to be allocated</param>
/// <param name="rp_data">Data to be set on init. If nullptr, buffer will be "empty"</param>
/// <param name="r_usage">Set usage of buffer</param>
/// <returns>Index buffer</returns>
FrameBuffer create_fb(FbRenderTargetType type, const int width, const int height, const int nChannels);

void fb_create_rb_for_depth(FrameBuffer* fb, GLenum format, int width, int height);
void _fb_attach_rb(FrameBuffer* fb, RenderBuffer* rb);

inline void use_fb(const FrameBuffer& r_fb)
{
    glBindFramebuffer(GL_FRAMEBUFFER, r_fb.Id);
}

inline void fb_unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// Deletes buffer
/// </summary>
/// <param name="r_vbo">Buffer to delete. Its Id will be set to 0</param>
void release_fb(FrameBuffer& r_fb);

#endif // !FRAME_BUFFER_H


