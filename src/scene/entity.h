#ifndef ENTITY_H
#define ENTITY_H

#include <glm/vec3.hpp>
#include "../renderer/data/mesh.h"
#include "../renderer/data/model.h"

typedef struct
{
	ModelHandle ModelHandle;
} EntityRenderData;

typedef struct
{
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	// TODO: entities should have components. Then a mesh should be a component
	EntityRenderData meshRendererData;

} Entity;


#endif // !ENTITY_H
