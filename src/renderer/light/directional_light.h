#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H
#include <glm/vec3.hpp>

typedef struct {
	glm::vec3 Direction;
	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;
} DirectionalLight;

#endif
