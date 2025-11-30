#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "texture.h"

typedef struct
{
	glm::vec3 Ambient;
	glm::vec3 Specular;
	glm::vec3 Diffuse;
	float Shininess;

	ShaderHandle Shader;

	TextureHandle AlbedoMap;
} Material;

typedef struct
{
	unsigned int Id;
	unsigned int GenerationId;
} MaterialHandle;

#endif // !MATERIAL_H
