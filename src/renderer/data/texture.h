#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

typedef struct
{
	unsigned int Texture;
	int Width, Height, NrChannels;
} Texture;

typedef struct
{
	unsigned int Id;
	unsigned int GenerationId;
} TextureHandle;


void create_texture(Texture* rpTexture, const char* rPath);

void create_texture(Texture* rpTexture, int n_channels, int width, int height);

inline void use_texture(Texture* rpTexture)
{
	glBindTexture(GL_TEXTURE_2D, rpTexture->Texture);
}

void release_texture(Texture* pTexture);


#endif // !TEXTURE_H
