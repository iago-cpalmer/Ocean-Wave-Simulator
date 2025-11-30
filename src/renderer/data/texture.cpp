#include "texture.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void create_texture(Texture* rpTexture, const char* rPath)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(rPath, &width, &height, &nChannels, 0);

	if (data)
	{
		if (nChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	rpTexture->NrChannels = nChannels;
	rpTexture->Width = width;
	rpTexture->Height = height;
	rpTexture->Texture = textureId;

	glBindTexture(GL_TEXTURE_2D, 0);
}

void create_texture(Texture* rpTexture, int n_channels, int width, int height)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (n_channels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}

	rpTexture->NrChannels = n_channels;
	rpTexture->Width = width;
	rpTexture->Height = height;
	rpTexture->Texture = textureId;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void release_texture(Texture* pTexture)
{
	glDeleteTextures(1, &pTexture->Texture);
}


