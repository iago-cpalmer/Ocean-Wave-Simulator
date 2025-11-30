#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include <glm/vec3.hpp>

typedef struct {
	glm::vec3 Position;
	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;

	float Constant;
	float Linear;
	float Quadratic;
} PointLight;

#endif